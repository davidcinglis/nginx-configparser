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

TEST_F(NginxStringConfigTest, SimpleConfig) {
    EXPECT_TRUE(ParseString("foo bar;")) << "error!";
    EXPECT_EQ(1, config_.statements_.size()) << "error!";
    EXPECT_EQ("foo", config_.statements_.at(0)->tokens_.at(0)) << "error";
}

TEST_F(NginxStringConfigTest, NestedConfig) {
    EXPECT_TRUE(ParseString("server { listen 80; }"));
}