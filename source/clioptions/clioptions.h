//
// Created by User on 11/22/2021.
//

#pragma once



#include <functional>   // std::function
#include <iostream>     // std::cout, std::endl
#include <map>          // std::map
#include <memory>       // std::unique_ptr
#include <string>       // std::string
#include <sstream>      // std::stringstream
#include <string_view>  // std::string_view
#include <variant>      // std::variant
#include <vector>       // std::vector
using namespace std;

template <class Opts>
struct CmdOpts : Opts
{
    using MyProp = std::variant<string Opts::*, int Opts::*, double Opts::*, bool Opts::*>;
    using MyArg = std::pair<string, MyProp>;

    ~CmdOpts() = default;

    Opts parse(int argc, char** argv)
    {
        vector<string_view> vargv(argv, argv+argc);
        for (int idx = 0; idx < argc; ++idx)
            for (auto& cbk : callbacks)
                cbk.second(idx, vargv);

        return static_cast<Opts>(*this);
    }

    static unique_ptr<CmdOpts> Create(std::initializer_list<MyArg> args)
    {

        auto cmdOpts = unique_ptr<CmdOpts>(new CmdOpts());
        for (auto arg : args) cmdOpts->register_callback(arg);
        return cmdOpts;
    }

private:
    using callback_t = function<void(int, const vector<string_view>&)>;
    map<string, callback_t> callbacks;

    CmdOpts() = default;
    CmdOpts(const CmdOpts&) = delete;
    CmdOpts(CmdOpts&&) = delete;
    CmdOpts& operator=(const CmdOpts&) = delete;
    CmdOpts& operator=(CmdOpts&&) = delete;

    auto register_callback(string name, MyProp prop)
    {
        callbacks[name] = [this, name, prop](int idx, const vector<string_view>& argv)
        {
            if (argv[idx] == name)
            {
                visit(
                        [this, idx, &argv](auto&& arg)
                        {
                            if (idx < argv.size() - 1)
                            {
                                stringstream value;
                                value << argv[idx+1];
                                value >> this->*arg;
                            }
                        },
                        prop);
            }
        };
    };

    auto register_callback(MyArg p) { return register_callback(p.first, p.second); }
};


namespace FONTTEST_OPTIONS{

    struct OptionsParsed
    {
        std::string fontPath {"consolas.ttf"};
        std::string frgShdrPath { "text.fs.glsl"};
        std::string vtxShdrPath { "text.vs.glsl"};

    };
    auto ParseOptions(int argc, char**argv)
    {
        std::unique_ptr<CmdOpts<OptionsParsed>> parser = CmdOpts<OptionsParsed>::Create(
                {
                        {"--fnt", &OptionsParsed::fontPath},
                        {"--vtx", &OptionsParsed::vtxShdrPath},
                        {"--frg", &OptionsParsed::frgShdrPath}
                });
        auto myopts = parser->parse(argc, argv);
        return myopts;
    }
}

/*
int main(int argc, const char* argv[])
{
    struct OptionsParsed
    {
        string stringOpt{};
        int intOpt{};
        bool boolOpt{};
    };

    auto parser = CmdOpts<OptionsParsed>::Create({
                                                  {"--opt1", &OptionsParsed::stringOpt },
                                                  {"--opt2", &OptionsParsed::intOpt},
                                                  {"--opt3", &OptionsParsed::boolOpt}});

    auto myopts = parser->parse(argc, argv);

    std::cout << "stringOpt = " << myopts.stringOpt << endl;
    std::cout << "intOpt = " << myopts.intOpt << endl;
    std::cout << "boolOpt = " << myopts.boolOpt << endl;
}
*/