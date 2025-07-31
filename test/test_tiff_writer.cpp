#include "tiff_writer_api.h"

#include <gtest/gtest.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include <zlib.h>

#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>

struct TestCase {
    std::string filename;
    int width;
    int height;
};

std::vector<uint8_t> read_file(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(in)), {});
}

std::string decompress_gzip(const std::vector<uint8_t>& compressed) {
    size_t decompressed_size = 8192;
    std::string result;
    result.resize(decompressed_size);

    z_stream strm = {};
    strm.next_in = const_cast<Bytef*>(compressed.data());
    strm.avail_in = compressed.size();
    strm.next_out = reinterpret_cast<Bytef*>(&result[0]);
    strm.avail_out = result.size();

    inflateInit2(&strm, 16 + MAX_WBITS); // gzip decoding
    int ret;
    while ((ret = inflate(&strm, Z_NO_FLUSH)) == Z_OK) {
        if (strm.avail_out == 0) {
            size_t old_size = result.size();
            result.resize(old_size * 2);
            strm.next_out = reinterpret_cast<Bytef*>(&result[old_size]);
            strm.avail_out = old_size;
        }
    }
    if (ret != Z_STREAM_END) {
        inflateEnd(&strm);
        throw std::runtime_error("Failed to decompress XML");
    }

    result.resize(strm.total_out);
    inflateEnd(&strm);
    return result;
}

std::vector<TestCase> load_test_cases_from_gzip_xml(const std::string& gz_path) {
    auto compressed = read_file(gz_path);
    std::string xml_str = decompress_gzip(compressed);

    xmlDocPtr doc = xmlReadMemory(xml_str.data(), xml_str.size(), "noname.xml", NULL, 0);
    if (!doc) throw std::runtime_error("Failed to parse XML");

    std::vector<TestCase> tests;
    xmlNodePtr root = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = root->children; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE && xmlStrEqual(node->name, BAD_CAST "test")) {
            TestCase tc;
            for (xmlNodePtr child = node->children; child; child = child->next) {
                if (child->type != XML_ELEMENT_NODE) continue;
                if (xmlStrEqual(child->name, BAD_CAST "filename")) {
                    tc.filename = (const char*)xmlNodeGetContent(child);
                } else if (xmlStrEqual(child->name, BAD_CAST "width")) {
                    tc.width = std::stoi((const char*)xmlNodeGetContent(child));
                } else if (xmlStrEqual(child->name, BAD_CAST "height")) {
                    tc.height = std::stoi((const char*)xmlNodeGetContent(child));
                }
            }
            tests.push_back(tc);
        }
    }
    xmlFreeDoc(doc);
    return tests;
}

std::filesystem::path GetTestDataDir() {
    auto const d = std::filesystem::path(TEST_DATA_DIR);
    if (!std::filesystem::exists(d)) {
        throw std::runtime_error("Test data directory does not exist: " + d.generic_string());
    }
    return d;
}

TEST(TiffWriterTest, RunFromZippedXmlCases) {
    auto const test_data_dir = GetTestDataDir();
    auto const path = test_data_dir / "test_cases.xml.gz";
    auto test_cases = load_test_cases_from_gzip_xml(path);

    for (const auto& tc : test_cases) {
        SCOPED_TRACE("Generating: " + tc.filename);
        bool result = tw_write_gray_ramp_tiff(tc.filename.c_str(), tc.width, tc.height);
        EXPECT_TRUE(result);
        EXPECT_TRUE(std::filesystem::exists(tc.filename));
    }
}
