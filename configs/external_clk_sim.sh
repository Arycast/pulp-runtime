#!/bin/bash -e

export PULPRT_TARGET=pulpissimo
export PULPRUN_TARGET=pulpissimo

if [  -n "${ZSH_VERSION:-}" ]; then
        DIR="$(readlink -f -- "${(%):-%x}")"
        scriptDir="$(dirname $DIR)"
else

    scriptDir="$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")"

fi

# ----------------------------------------------------
# Help Menu Option
# ----------------------------------------------------
if [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
    echo "PULPissimo Environment Configuration Script"
    echo ""
    echo "Usage:"
    echo "  source $(basename "${BASH_SOURCE[0]}")                        Fallback to defaults (10MHz PER, 20MHz SOC, 115200 Baud)"
    echo "  source $(basename "${BASH_SOURCE[0]}") [PER] [SOC]            Override frequencies, use default 115200 Baud"
    echo "  source $(basename "${BASH_SOURCE[0]}") [PER] [SOC] [BAUD]     Override frequencies and UART Baudrate"
    echo ""
    echo "Examples (used for latest tapeout simulation):"
    echo "  source $(basename "${BASH_SOURCE[0]}") 32768 32768"
    echo "  source $(basename "${BASH_SOURCE[0]}") 32768 32768 1000"
    echo ""
    
    # Safe exit strategy so sourcing the script doesn't close the terminal
    (return 0 2>/dev/null) && return 0 || exit 0
fi

source $scriptDir/common.sh

# ----------------------------------------------------
# Frequency Configuration (with Argument Overrides)
# ----------------------------------------------------
if [ $# -eq 2 ] || [ $# -eq 3 ]; then
    echo "--> Custom configuration detected from arguments."
    PER_FREQ=$1
    SOC_FREQ=$2
    
    # If a 3rd argument is provided, use it for baudrate; otherwise, use default
    if [ $# -eq 3 ]; then
        BAUDRATE=$3
    else
        BAUDRATE=115200
    fi
else
    echo "--> Using default frequencies and baudrate (Arjuna/Nusacore)."
    PER_FREQ=10000000
    SOC_FREQ=20000000
    BAUDRATE=115200
fi

# Print the frequencies to the terminal
echo "Setting PER Frequency to: $PER_FREQ Hz"
echo "Setting SOC Frequency to: $SOC_FREQ Hz"
echo "Setting UART Baudrate to: $BAUDRATE"

export CONFIG_IO_UART_BAUDRATE=$BAUDRATE
export PULPRUN_PLATFORM=fpga  # Forcing the pulp-runtime to not initiate FLL
export io=uart

# Inject the variables into the flags
export PULPRT_CONFIG_ASFLAGS="-DARCHI_FPGA_PER_FREQUENCY=$PER_FREQ -DARCHI_FPGA_SOC_FREQUENCY=$SOC_FREQ"
export PULPRT_CONFIG_CFLAGS="-DARCHI_FPGA_PER_FREQUENCY=$PER_FREQ -DARCHI_FPGA_SOC_FREQUENCY=$SOC_FREQ"
export PULPRT_CONFIG_CXXFLAGS="-DARCHI_FPGA_PER_FREQUENCY=$PER_FREQ -DARCHI_FPGA_SOC_FREQUENCY=$SOC_FREQ"
