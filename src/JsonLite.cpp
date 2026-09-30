#include "pokemon/JsonLite.hpp"

#include <cmath>
#include <iomanip>
#include <istream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace pokemon {
namespace {
class Parser {
public:
    explicit Parser(const std::string& text) : text_(text) {}

    JsonValue parse() {
        skipWhitespace();
        JsonValue value = parseValue();
        skipWhitespace();
        if (position_ != text_.size()) fail("caracteres inattendus apres la valeur JSON");
        return value;
    }

private:
    const std::string& text_;
    std::size_t position_ = 0;

    [[noreturn]] void fail(const std::string& message) const {
        throw std::runtime_error("JSON invalide a la position " + std::to_string(position_) + ": " + message);
    }

    void skipWhitespace() {
        while (position_ < text_.size()) {
            const char c = text_[position_];
            if (c != ' ' && c != '\n' && c != '\r' && c != '\t') break;
            ++position_;
        }
    }

    bool consume(char expected) {
        skipWhitespace();
        if (position_ < text_.size() && text_[position_] == expected) {
            ++position_;
            return true;
        }
        return false;
    }

    void expect(char expected) {
        if (!consume(expected)) fail(std::string("'" ) + expected + "' attendu");
    }

    JsonValue parseValue() {
        skipWhitespace();
        if (position_ >= text_.size()) fail("valeur attendue");
        const char c = text_[position_];
        if (c == '{') return parseObject();
        if (c == '[') return parseArray();
        if (c == '"') return JsonValue(parseString());
        if (c == '-' || (c >= '0' && c <= '9')) return JsonValue(parseNumber());
        if (match("true")) return JsonValue(true);
        if (match("false")) return JsonValue(false);
        if (match("null")) return JsonValue();
        fail("valeur JSON inconnue");
    }

    bool match(const char* token) {
        const std::size_t start = position_;
        while (*token != '\0') {
            if (position_ >= text_.size() || text_[position_] != *token) {
                position_ = start;
                return false;
            }
            ++position_;
            ++token;
        }
        return true;
    }

    JsonValue parseObject() {
        expect('{');
        JsonValue::Object object;
        skipWhitespace();
        if (consume('}')) return JsonValue(std::move(object));
        while (true) {
            skipWhitespace();
            if (position_ >= text_.size() || text_[position_] != '"') fail("cle d'objet attendue");
            std::string key = parseString();
            expect(':');
            object.emplace(std::move(key), parseValue());
            if (consume('}')) break;
            expect(',');
        }
        return JsonValue(std::move(object));
    }

    JsonValue parseArray() {
        expect('[');
        JsonValue::Array array;
        skipWhitespace();
        if (consume(']')) return JsonValue(std::move(array));
        while (true) {
            array.push_back(parseValue());
            if (consume(']')) break;
            expect(',');
        }
        return JsonValue(std::move(array));
    }

    std::string parseString() {
        if (position_ >= text_.size() || text_[position_] != '"') fail("chaine attendue");
        ++position_;
        std::string result;
        while (position_ < text_.size()) {
            const char c = text_[position_++];
            if (c == '"') return result;
            if (c != '\\') {
                result += c;
                continue;
            }
            if (position_ >= text_.size()) fail("echappement incomplet");
            const char escaped = text_[position_++];
            switch (escaped) {
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case '/': result += '/'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                default: fail("echappement non supporte");
            }
        }
        fail("chaine non terminee");
    }

    double parseNumber() {
        const std::size_t start = position_;
        if (text_[position_] == '-') ++position_;
        while (position_ < text_.size() && text_[position_] >= '0' && text_[position_] <= '9') ++position_;
        if (position_ < text_.size() && text_[position_] == '.') {
            ++position_;
            while (position_ < text_.size() && text_[position_] >= '0' && text_[position_] <= '9') ++position_;
        }
        if (position_ < text_.size() && (text_[position_] == 'e' || text_[position_] == 'E')) {
            ++position_;
            if (position_ < text_.size() && (text_[position_] == '+' || text_[position_] == '-')) ++position_;
            while (position_ < text_.size() && text_[position_] >= '0' && text_[position_] <= '9') ++position_;
        }
        try {
            return std::stod(text_.substr(start, position_ - start));
        } catch (...) {
            fail("nombre invalide");
        }
    }
};

std::string escapeString(const std::string& value) {
    std::string result;
    for (const char c : value) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += c; break;
        }
    }
    return result;
}

std::string dumpValue(const JsonValue& value, int indent, int depth) {
    const std::string padding(static_cast<std::size_t>(depth * indent), ' ');
    const std::string childPadding(static_cast<std::size_t>((depth + 1) * indent), ' ');
    switch (value.kind()) {
        case JsonValue::Kind::Null: return "null";
        case JsonValue::Kind::Boolean: return value.asBoolean() ? "true" : "false";
        case JsonValue::Kind::Number: {
            const double number = value.asNumber();
            if (std::floor(number) == number) return std::to_string(static_cast<long long>(number));
            std::ostringstream stream;
            stream << std::setprecision(15) << number;
            return stream.str();
        }
        case JsonValue::Kind::String: return "\"" + escapeString(value.asString()) + "\"";
        case JsonValue::Kind::Array: {
            const auto& array = value.asArray();
            if (array.empty()) return "[]";
            std::string result = "[";
            for (std::size_t i = 0; i < array.size(); ++i) {
                result += "\n" + childPadding + dumpValue(array[i], indent, depth + 1);
                if (i + 1 < array.size()) result += ',';
            }
            return result + "\n" + padding + "]";
        }
        case JsonValue::Kind::Object: {
            const auto& object = value.asObject();
            if (object.empty()) return "{}";
            std::string result = "{";
            std::size_t index = 0;
            for (const auto& [key, child] : object) {
                result += "\n" + childPadding + "\"" + escapeString(key) + "\": " + dumpValue(child, indent, depth + 1);
                if (++index < object.size()) result += ',';
            }
            return result + "\n" + padding + "}";
        }
    }
    return "null";
}
}

JsonValue::JsonValue() = default;
JsonValue::JsonValue(bool value) : kind_(Kind::Boolean), boolean_(value) {}
JsonValue::JsonValue(double value) : kind_(Kind::Number), number_(value) {}
JsonValue::JsonValue(std::string value) : kind_(Kind::String), string_(std::move(value)) {}
JsonValue::JsonValue(Array value) : kind_(Kind::Array), array_(std::move(value)) {}
JsonValue::JsonValue(Object value) : kind_(Kind::Object), object_(std::move(value)) {}

JsonValue::Kind JsonValue::kind() const { return kind_; }
bool JsonValue::isNull() const { return kind_ == Kind::Null; }
bool JsonValue::isBoolean() const { return kind_ == Kind::Boolean; }
bool JsonValue::isNumber() const { return kind_ == Kind::Number; }
bool JsonValue::isString() const { return kind_ == Kind::String; }
bool JsonValue::isArray() const { return kind_ == Kind::Array; }
bool JsonValue::isObject() const { return kind_ == Kind::Object; }

bool JsonValue::asBoolean() const { if (!isBoolean()) throw std::runtime_error("JSON: booleen attendu"); return boolean_; }
double JsonValue::asNumber() const { if (!isNumber()) throw std::runtime_error("JSON: nombre attendu"); return number_; }
int JsonValue::asInt() const { return static_cast<int>(asNumber()); }
const std::string& JsonValue::asString() const { if (!isString()) throw std::runtime_error("JSON: chaine attendue"); return string_; }
const JsonValue::Array& JsonValue::asArray() const { if (!isArray()) throw std::runtime_error("JSON: tableau attendu"); return array_; }
const JsonValue::Object& JsonValue::asObject() const { if (!isObject()) throw std::runtime_error("JSON: objet attendu"); return object_; }
JsonValue::Array& JsonValue::asArray() { if (!isArray()) throw std::runtime_error("JSON: tableau attendu"); return array_; }
JsonValue::Object& JsonValue::asObject() { if (!isObject()) throw std::runtime_error("JSON: objet attendu"); return object_; }

bool JsonValue::contains(const std::string& key) const {
    return isObject() && object_.find(key) != object_.end();
}
const JsonValue& JsonValue::at(const std::string& key) const {
    if (!isObject()) throw std::runtime_error("JSON: objet attendu pour la cle " + key);
    const auto it = object_.find(key);
    if (it == object_.end()) throw std::runtime_error("JSON: cle manquante: " + key);
    return it->second;
}
JsonValue& JsonValue::operator[](const std::string& key) {
    if (isNull()) kind_ = Kind::Object;
    if (!isObject()) throw std::runtime_error("JSON: objet attendu");
    return object_[key];
}

std::string JsonValue::dump(int indent) const { return dumpValue(*this, indent, 0); }
JsonValue JsonValue::parse(const std::string& text) { return Parser(text).parse(); }
JsonValue JsonValue::parse(std::istream& input) {
    std::ostringstream stream;
    stream << input.rdbuf();
    return parse(stream.str());
}

} // namespace pokemon
