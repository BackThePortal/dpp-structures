//
// Created by backo on 4/8/2024.
//

#include "internal_json_file.h"

using namespace dpp_structures;

internal_file_value::~internal_file_value() { delete this->string; }
