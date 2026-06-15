#!/bin/bash -e

export PULPRT_TARGET=pulpissimo
export PULPRUN_TARGET=pulpissimo

if [  -n "${ZSH_VERSION:-}" ]; then 
    DIR="$(readlink -f -- "${(%):-%x}")"
    scriptDir="$(dirname $DIR)"
else

    scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

fi

source $scriptDir/../../common.sh

export PULPRUN_PLATFORM=fpga

# Define the frequencies once (Arjuna or Nusacore uses 15MHz SoC Frequency)
PER_FREQ=10000000
SOC_FREQ=15000000

# Print the frequencies to the terminal
echo "Setting PER Frequency to: $PER_FREQ Hz"
echo "Setting SOC Frequency to: $SOC_FREQ Hz"

# Inject the variables into the flags
export PULPRT_CONFIG_ASFLAGS="-DARCHI_FPGA_PER_FREQUENCY=$PER_FREQ -DARCHI_FPGA_SOC_FREQUENCY=$SOC_FREQ"
export PULPRT_CONFIG_CFLAGS="-DARCHI_FPGA_PER_FREQUENCY=$PER_FREQ -DARCHI_FPGA_SOC_FREQUENCY=$SOC_FREQ"
export PULPRT_CONFIG_CXXFLAGS="-DARCHI_FPGA_PER_FREQUENCY=$PER_FREQ -DARCHI_FPGA_SOC_FREQUENCY=$SOC_FREQ"

export io=uart
