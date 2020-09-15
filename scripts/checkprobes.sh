#!/bin/bash

IGNORED=("HBreaker HWindow HStreamProcessor HNetworkProcessor HTimer HFft HFir HOutput HNotchBiQuad HPeakingEQBiQuad HLowpassBiQuad HAllpassBiQuad HHighShelvingBiQuad HHighpassBiQuad HLowShelvingBiQuad HBandpassBiQuad HBiQuad HNullWriter HNullReader HSineGenerator HCosineGenerator HMute HMux HDeMux HLocalOscillator HKaiserBessel HLowpassKaiserBessel HHighpassKaiserBessel HBandpassKaiserBessel HBandstopKaiserBessel HGoertzelOutput HMemoryWriter HMetrics HObject HBufferedWriter HChunkWriter HCustomWriter HFade HFftOutput HFileWriter HGenerator HFileReader HInputWriter HMemoryReader HNetworkReader HNetworkWriter HProcessor HSignalLevelOutput HSoundcardWriter HSplitter HVfo HWav HWavWriter")

for HEADER in $(ls ../hardt/libhardt/include); do
  CLASSNAME=$(cat ../hardt/libhardt/include/$HEADER | grep 'class ' | grep -v 'template' | cut -d ' ' -f 2 | tr -d '\n')

  IGNORE=0
  for IGNOREDCLASS in $IGNORED; do
    if [ "$CLASSNAME" == "$IGNOREDCLASS" ]; then
      IGNORE=1
    fi
  done

  if [ $IGNORE -eq 0 ]; then
    MISSING=0
    cat ../hardt/libhardt/include/$HEADER  | grep -E "^ +${CLASSNAME}\(" | grep -v -E ".+HProbe<.+>*.+" | while read -r CONSTRUCTOR; do
      if [ $MISSING -eq 0 ]; then
        echo
        echo "Header: $HEADER"
        echo "Classname: $CLASSNAME"
        echo "Reason: Missing declaration"
        MISSING=1
        export SUM=$((SUM+1))
      fi
      echo "  $CONSTRUCTOR"
    done

   # Todo: check implementation

  fi

done

