#include "atomizer_client.hpp"
#include "bech32/bech32.h"
#include "bech32/util/strencodings.h"
#include "client.hpp"
#include "crypto/sha256.h"
#include "hu_bme_mit_opencbdc_OpenCBDCJavaClient.h"
#include "twophase_client.hpp"
#include "uhs/transaction/messages.hpp"
#include "util/common/config.hpp"
#include "util/serialization/util.hpp"

#include <future>
#include <iostream>
#include <string>

static constexpr auto bits_per_byte = 8;
static constexpr auto bech32_bits_per_symbol = 5;


auto mint_command(cbdc::client& client, const std::vector<std::string>& args)
    -> bool {
    static constexpr auto min_mint_arg_count = 7;
    if(args.size() < min_mint_arg_count) {
        std::cerr << "Mint requires args <n outputs> <output value>"
                  << std::endl;
        return false;
    }

    const auto n_outputs = std::stoull(args[5]);
    const auto output_val = std::stoul(args[6]);

    const auto mint_tx
        = client.mint(n_outputs, static_cast<uint32_t>(output_val));
    std::cout << cbdc::to_string(cbdc::transaction::tx_id(mint_tx))
              << std::endl;
    return true;
}

auto decode_address(const std::string& addr_str)
    -> std::optional<cbdc::hash_t> {
    // TODO: if/when bech32m is merged into Bitcoin Core, switch to that.
    //       see: https://github.com/bitcoin/bitcoin/pull/20861
    // TODO: move address encoding/decoding into cbdc::client.
    const auto [hrp, enc_data] = bech32::Decode(addr_str);
    if(hrp != cbdc::config::bech32_hrp) {
        std::cout << "Invalid address encoding" << std::endl;
        return std::nullopt;
    }
    auto data = std::vector<uint8_t>();
    ConvertBits<bech32_bits_per_symbol, bits_per_byte, false>(
        [&](uint8_t c) {
            data.push_back(c);
        },
        enc_data.begin(),
        enc_data.end());

    auto pubkey = cbdc::hash_t();
    if(data[0] != static_cast<uint8_t>(cbdc::client::address_type::public_key)
       || data.size() != pubkey.size() + 1) {
        std::cout << "Address is not a supported type" << std::endl;
        return std::nullopt;
    }
    data.erase(data.begin());
    std::copy_n(data.begin(), pubkey.size(), pubkey.begin());

    return pubkey;
}

void print_tx_result(const std::optional<cbdc::transaction::full_tx>& tx,
                     const std::optional<cbdc::sentinel::response>& resp,
                     const cbdc::hash_t& pubkey) {
    std::cout << "tx_id:" << std::endl
              << cbdc::to_string(cbdc::transaction::tx_id(tx.value()))
              << std::endl;
    const auto inputs = cbdc::client::export_send_inputs(tx.value(), pubkey);
    for(const auto& inp : inputs) {
        auto buf = cbdc::make_buffer(inp);
        std::cout << "Data for recipient importinput:" << std::endl
                  << buf.to_hex() << std::endl;
    }

    if(resp.has_value()) {
        std::cout << "Sentinel responded: "
                  << cbdc::sentinel::to_string(resp.value().m_tx_status)
                  << std::endl;
        if(resp.value().m_tx_error.has_value()) {
            std::cout << "Validation error: "
                      << cbdc::transaction::validation::to_string(
                             resp.value().m_tx_error.value())
                      << std::endl;
        }
    }
}

auto send_command(cbdc::client& client, const std::vector<std::string>& args)
    -> bool {
    static constexpr auto min_send_arg_count = 7;
    if(args.size() < min_send_arg_count) {
        std::cerr << "Send requires args <value> <pubkey>" << std::endl;
        return false;
    }

    const auto value = std::stoul(args[5]);
    static constexpr auto address_arg_idx = 6;
    auto pubkey = decode_address(args[address_arg_idx]);
    if(!pubkey.has_value()) {
        std::cout << "Could not decode address" << std::endl;
        return false;
    }

    const auto [tx, resp]
        = client.send(static_cast<uint32_t>(value), pubkey.value());
    if(!tx.has_value()) {
        std::cout << "Could not generate valid send tx." << std::endl;
        return false;
    }

    print_tx_result(tx, resp, pubkey.value());
    return true;
}

auto fan_command(cbdc::client& client, const std::vector<std::string>& args)
    -> bool {
    static constexpr auto min_fan_arg_count = 8;
    if(args.size() < min_fan_arg_count) {
        std::cerr << "Fan requires args <count> <value> <pubkey>" << std::endl;
        return false;
    }

    const auto value = std::stoul(args[6]);
    const auto count = std::stoul(args[5]);

    static constexpr auto address_arg_idx = 7;
    auto pubkey = decode_address(args[address_arg_idx]);
    if(!pubkey.has_value()) {
        std::cout << "Could not decode address" << std::endl;
        return false;
    }

    const auto [tx, resp] = client.fan(static_cast<uint32_t>(count),
                                       static_cast<uint32_t>(value),
                                       pubkey.value());
    if(!tx.has_value()) {
        std::cout << "Could not generate valid send tx." << std::endl;
        return false;
    }

    print_tx_result(tx, resp, pubkey.value());
    return true;
}

void newaddress_command(cbdc::client& client) {
    const auto addr = client.new_address();
    auto addr_vec
        = std::vector<uint8_t>(sizeof(cbdc::client::address_type::public_key)
                               + std::tuple_size<decltype(addr)>::value);
    addr_vec[0] = static_cast<uint8_t>(cbdc::client::address_type::public_key);
    std::copy_n(addr.begin(),
                addr.size(),
                addr_vec.begin()
                    + sizeof(cbdc::client::address_type::public_key));
    auto data = std::vector<uint8_t>();
    ConvertBits<bits_per_byte, bech32_bits_per_symbol, true>(
        [&](uint8_t c) {
            data.push_back(c);
        },
        addr_vec.begin(),
        addr_vec.end());
    std::cout << bech32::Encode(cbdc::config::bech32_hrp, data) << std::endl;
}

auto importinput_command(cbdc::client& client,
                         const std::vector<std::string>& args) -> bool {
    static constexpr auto input_arg_idx = 5;
    auto buffer = cbdc::buffer::from_hex(args[input_arg_idx]);
    if(!buffer.has_value()) {
        std::cout << "Invalid input encoding." << std::endl;
        return false;
    }

    auto in = cbdc::from_buffer<cbdc::transaction::input>(buffer.value());
    if(!in.has_value()) {
        std::cout << "Invalid input" << std::endl;
        return false;
    }
    client.import_send_input(in.value());
    return true;
}

auto confirmtx_command(cbdc::client& client,
                       const std::vector<std::string>& args) -> bool {
    const auto tx_id = cbdc::hash_from_hex(args[5]);
    auto success = client.confirm_transaction(tx_id);
    if(!success) {
        std::cout << "Unknown TXID" << std::endl;
        return false;
    }
    std::cout << "Confirmed. Balance: "
              << cbdc::client::print_amount(client.balance())
              << " UTXOs: " << client.utxo_count() << std::endl;
    return true;
}


JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_send(JNIEnv* env,
                                                 jobject obj,
                                                 jobjectArray arr) {
    std::ostringstream strCout;
    

    std::vector<std::string> args = {};
    jsize argc = env->GetArrayLength(arr);
    strCout << "transaction params: ";
    for(int i = 0; i < argc; i++) {
        args.push_back(
            env->GetStringUTFChars((jstring)env->GetObjectArrayElement(arr, i),
                                   NULL));
    }
    for(int i = 0; i < argc; i++) {
        strCout << args[i];
        strCout << " ";
    }
    strCout << std::endl;

    return env->NewStringUTF(strCout.str().c_str());
}
/* 
JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_mint(JNIEnv* env,
                                                 jobject obj,
                                                 jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_fan(JNIEnv* env,
                                                jobject obj,
                                                jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_sync(JNIEnv* env,
                                                 jobject obj,
                                                 jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_newAddress(JNIEnv* env,
                                                       jobject obj,
                                                       jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_info(JNIEnv* env,
                                                 jobject obj,
                                                 jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_importInput(JNIEnv* env,
                                                        jobject obj,
                                                        jobjectArray arr) {
    return env->NewStringUTF("");
}

JNIEXPORT jstring JNICALL
Java_hu_bme_mit_opencbdc_OpenCBDCJavaClient_confirmTx(JNIEnv* env,
                                                      jobject obj,
                                                      jobjectArray arr) {
    return env->NewStringUTF("");
}
 */