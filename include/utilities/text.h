//
// Created by backo on 27/6/2024.
//

#ifndef DPP_STRUCTURES_TEXT_H
#define DPP_STRUCTURES_TEXT_H

#include <string>
#include <vector>
#include <generator>

namespace dpp_structures {

    // Discord markdown utilities

    inline constexpr int CHARACTER_LIMIT = 2000;

    inline std::string bold(const std::string& s) { return "**" + s + "**"; }

    inline std::string italic(const std::string& s) { return "_" + s + "_"; }

    inline std::string underline(const std::string& s) { return "__" + s + "__"; }

    inline std::string strikethrough(const std::string& s) { return "~~" + s + "~~"; }

    inline std::string code(const std::string& s) { return "`" + s + "`"; }

    inline std::string codeblock(const std::string& s) { return "```\n" + s + "\n```"; }

    inline std::string codeblock(const std::string& s, const std::string& lang) {
        return "```" + lang + "\n" + s + "\n```";
    }

    inline std::string link(const std::string& text, const std::string& link) { return "[" + text + "](" + link + ")"; }


    // String utilities

    inline std::string capitalize(const std::string& s) {
        std::string c;
        c = (char) toupper(s[0]);
        return c + s.substr(1);
    }

    inline std::string join(const std::vector<std::string>& v, const std::string& sep) {
        std::string r;

        int i = 0;
        for (const auto& s : v) {
            ++i;
            r.append(s);
            if (i != v.size()) r.append(sep);
        }

        return r;
    }

    inline std::string join_paths(const std::string& base, const std::string& add) {
        bool baseEnds = base.ends_with('/');
        bool addEnds = add.starts_with('/');
        if (addEnds != baseEnds) return base + add;
        if (!addEnds) return base + "/" + add;
        else {
            return base.substr(0, base.size() - 1) + add;
        }
    } 
    
    std::vector<std::string> split_string(const std::string& str, const std::string& delimiter);

    
    

}
#endif//DPP_STRUCTURES_TEXT_H
