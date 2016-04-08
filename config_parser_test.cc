#include <sstream>
#include <string>

#include "gtest/gtest.h"
#include "config_parser.h"

class NginxStringConfigTest : public ::testing::Test {
    protected:
        bool ParseString(const std::string &config_string) {
            std::stringstream config_stream(config_string);
            return parser_.Parse(&config_stream, &config_);
        }
        NginxConfigParser parser_;
        NginxConfig config_;

};

TEST_F(NginxStringConfigTest, SyntaxErrorConfig) {
    EXPECT_FALSE(ParseString("foo bar")) << "Error: failed to detect missing semicolon.";
    EXPECT_FALSE(ParseString("server { listen 80;")) << "Error: failed to detect missing }";
    EXPECT_FALSE(ParseString("listen 80; }")) << "Error: failed to detect missing {";
    EXPECT_FALSE(ParseString("server {listen 80 }")) << "Error: failed to detect missing semicolon within braces";
    EXPECT_FALSE(ParseString("server { \n server { listen 80; }")) << "Error: failed to detect error in nested braces";
}

TEST_F(NginxStringConfigTest, ValidSyntaxConfig) {
    EXPECT_TRUE(ParseString("foo bar;")) << "Error: failed to parse valid string correctly";
    EXPECT_EQ(1, config_.statements_.size()) << "Error: parsed token into incorrect number of statements";
    EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0)) << "Error: tokenized statement incorrectly";
}

TEST_F(NginxStringConfigTest, DoubleNestedConfig) {
    EXPECT_TRUE(ParseString("server { \n server { listen 80; } \n }")) << "Error: failed to parse doubly nested braces correctly";
}
