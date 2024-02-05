#pragma once

#include "vmt.h"

const unsigned int PAINT_HOOK_INDEX = 14;
const unsigned int CLIENTMODESHARED_HOOK_INDEX = 21;

extern vmt::vmthook* ivengineclient_hook; 
extern vmt::vmthook* clientmodeshared_hook;