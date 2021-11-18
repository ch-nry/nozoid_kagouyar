#!/bin/bash

cp m_VCO1.h m_VCO2.h
cp m_VCO1.h m_VCO3.h
sed 's/VCO1/VCO2/g' m_VCO2.h -i
sed 's/VCO1/VCO3/g' m_VCO3.h -i
cp m_LFO1.h m_LFO2.h
cp m_LFO1.h m_LFO3.h
sed 's/LFO1/LFO2/g' m_LFO2.h -i
sed 's/LFO1/LFO3/g' m_LFO3.h -i
sed 's/LFO4/LFO5/g' m_LFO2.h -i
sed 's/LFO4/LFO6/g' m_LFO3.h -i
make clean
make -j 7
rm m_VCO2.h
rm m_VCO3.h
rm m_LFO2.h
rm m_LFO3.h

if [ -f build/Kagouyar.bin ]; then
    echo "========================================"
    echo "=============== BUILD OK ==============="
    echo "========================================"
    if (($#>0)); then
        make program-dfu
    else
        make program
	fi
else
    echo "========================================"
    echo "============== BUILD FAIL =============="
    echo "========================================"
fi
