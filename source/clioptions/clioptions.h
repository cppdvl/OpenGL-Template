//
// Created by User on 11/22/2021.
//

#pragma once

namespace Tools::cliparse
{

    template <class OptionsPlaceHolder>
    struct CommandLineOptions : public OptionsPlaceHolder {

        using MyProp = std::variant
    };

}

#endif //OPENGLTEMPLATE_CLIOPTIONS_H
