#pragma once
#include <systemc.h>
#include "interface/ahb_master_if.h"
#include "interface/ahb_slave_if.h"

// simulate a simple dma controller
class DMACtrl : public AHBMasterIF, public AHBSlaveIF {};
