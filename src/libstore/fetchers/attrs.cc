#include "attrs.hh"
#include "fetchers.hh"

#include <nlohmann/json.hpp>

namespace nix::fetchers {

Attrs jsonToAttrs(const nlohmann::json & json)
{
    Attrs attrs;

    for (auto & i : json.items()) {
        if (i.value().is_number())
            attrs.emplace(i.key(), i.value().get<int64_t>());
        else if (i.value().is_string())
            attrs.emplace(i.key(), i.value().get<std::string>());
        else
            throw Error("unsupported input attribute type in lock file");
    }

    return attrs;
}

nlohmann::json attrsToJson(const Attrs & attrs)
{
    nlohmann::json json;
    for (auto & attr : attrs) {
        if (auto v = std::get_if<int64_t>(&attr.second)) {
            json[attr.first] = *v;
        } else if (auto v = std::get_if<std::string>(&attr.second)) {
            json[attr.first] = *v;
        } else abort();
    }
    return json;
}

std::optional<std::string> maybeGetStrAttr(const Attrs & attrs, const std::string & name)
{
    auto i = attrs.find(name);
    if (i == attrs.end()) return {};
    if (auto v = std::get_if<std::string>(&i->second))
        return *v;
    throw Error("input attribute '%s' is not a string", name);
}

std::string getStrAttr(const Attrs & attrs, const std::string & name)
{
    auto s = maybeGetStrAttr(attrs, name);
    if (!s)
        throw Error("input attribute '%s' is missing", name);
    return *s;
}

std::optional<int64_t> maybeGetIntAttr(const Attrs & attrs, const std::string & name)
{
    auto i = attrs.find(name);
    if (i == attrs.end()) return {};
    if (auto v = std::get_if<int64_t>(&i->second))
        return *v;
    throw Error("input attribute '%s' is not a string", name);
}

int64_t getIntAttr(const Attrs & attrs, const std::string & name)
{
    auto s = maybeGetIntAttr(attrs, name);
    if (!s)
        throw Error("input attribute '%s' is missing", name);
    return *s;
}

}