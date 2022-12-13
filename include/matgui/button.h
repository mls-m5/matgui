/*
 * Button.h
 *
 *  Created on: 22 jan 2015
 *      Author: Mattias Larsson Sköld
 */

#pragma once
#include "fontview.h"
#include "view.h"
#include <memory>
#include <string>

namespace matgui {

class Button : public View {
public:
    Button(std::string label = "");
    virtual ~Button();

    void draw() override;

    void label(const std::string text);

    const std::string &label();

    void font(Font font);

private:
    FontView _fontView;
};

} // namespace matgui
