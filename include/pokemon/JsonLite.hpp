#pragma once

#include <cstddef>
#include <iosfwd>
#include <map>
#include <string>
#include <vector>

namespace pokemon {

/**
 * Représente une valeur JSON minimale utilisée par les fichiers de données du projet.
 *
 * Entrées:
 *   Une valeur peut être nulle, booléenne, numérique, textuelle, un tableau ou un objet.
 *
 * Sortie:
 *   JsonValue: arbre JSON indépendant de toute bibliothèque externe.
 */
class JsonValue {
public:
    enum class Kind { Null, Boolean, Number, String, Array, Object };
    using Array = std::vector<JsonValue>;
    using Object = std::map<std::string, JsonValue>;

    JsonValue();
    explicit JsonValue(bool value);
    explicit JsonValue(double value);
    explicit JsonValue(std::string value);
    explicit JsonValue(Array value);
    explicit JsonValue(Object value);

    Kind kind() const;
    bool isNull() const;
    bool isBoolean() const;
    bool isNumber() const;
    bool isString() const;
    bool isArray() const;
    bool isObject() const;

    bool asBoolean() const;
    double asNumber() const;
    int asInt() const;
    const std::string& asString() const;
    const Array& asArray() const;
    const Object& asObject() const;
    Array& asArray();
    Object& asObject();

    bool contains(const std::string& key) const;
    const JsonValue& at(const std::string& key) const;
    JsonValue& operator[](const std::string& key);

    std::string dump(int indent = 2) const;

    static JsonValue parse(const std::string& text);
    static JsonValue parse(std::istream& input);

private:
    Kind kind_ = Kind::Null;
    bool boolean_ = false;
    double number_ = 0.0;
    std::string string_;
    Array array_;
    Object object_;
};

} // namespace pokemon
