#!/bin/bash

IGNORED=("HBreaker HWindow HStreamProcessor HNetworkProcessor HTimer HFft HFir HOutput HNotchBiQuad HPeakingEQBiQuad HLowpassBiQuad HAllpassBiQuad HHighShelvingBiQuad HHighpassBiQuad HLowShelvingBiQuad HBandpassBiQuad HBiQuad HNullWriter HNullReader HSineGenerator HCosineGenerator HMute HMux HDeMux HLocalOscillator HKaiserBessel HLowpassKaiserBessel HHighpassKaiserBessel HBandpassKaiserBessel HBandstopKaiserBessel HGoertzelOutput HMemoryWriter HMetrics HObject HBufferedWriter HChunkWriter HCustomWriter HFade HFftOutput HFileWriter HGenerator HInputWriter HMemoryReader HNetworkReader HNetworkWriter HProcessor HSignalLevelOutput HSoundcardWriter HSplitter HVfo HWav HWavWriter HComplex2MultiplexedConverter HComplexConverter HConverter HMultiplexed2ComplexConverter HProbe HSwitch HTypeConverter HCascadedBiQuadFilter")

for HEADER in $(ls ../hardt/libhardt/include); do
  CLASSNAME=$(cat ../hardt/libhardt/include/$HEADER | grep 'class ' | grep -v 'template' | cut -d ' ' -f 2 | tr -d '\n:')

  IGNORE=0
  for IGNOREDCLASS in $IGNORED; do
    if [ "$CLASSNAME" == "$IGNOREDCLASS" ]; then
      IGNORE=1
    fi
  done

  if [ $IGNORE -eq 0 ]; then
    MISSING=0
    cat ../hardt/libhardt/include/${HEADER}  | grep -E "^ +${CLASSNAME}\(" | grep -v -E ".+HProbe<.+>*.+" | while read -r CONSTRUCTOR; do
      if [ $MISSING -eq 0 ]; then
        echo
        echo "Header: ../hardt/libhardt/include/$HEADER"
        echo "Classname: $CLASSNAME"
        echo "  Reason: Missing declaration"
        MISSING=1
      fi
      echo "  $CONSTRUCTOR"
    done

    MISSING=0
    cat ../hardt/libhardt/include/${HEADER}  | grep -E "^ +${CLASSNAME}\(" | grep -E ".+HProbe<.+>*.+" | while read -r CONSTRUCTOR; do
      IMPLEMENTATION=$(find ../hardt/libhardt/ | grep $(echo $HEADER | sed 's/\.h/.cpp/'))

      cat $IMPLEMENTATION | grep 'probe->Write' &> /dev/null
      if [ $? -ne 0 ]; then

        cat ../hardt/libhardt/include/$HEADER | grep 'probe->Write' &> /dev/null
        if [ $? -ne 0 ]; then

          cat $IMPLEMENTATION | grep 'HFilter' | grep -E "^ +HFilter<T>(.+, probe).+" &> /dev/null
          if [ $? -ne 0 ]; then


            cat ../hardt/libhardt/include/$HEADER | grep 'HFilter' | grep -E "^ +HFilter<T>(.+, probe).+" &> /dev/null
            if [ $? -ne 0 ]; then

              cat $IMPLEMENTATION | grep 'HGain' | grep -E "^ +HGain<T>(.+, probe).+" &> /dev/null
              if [ $? -ne 0 ]; then

                cat ../hardt/libhardt/include/$HEADER | grep 'HGain' | grep -E "^ +HGain<T>(.+, probe).+" &> /dev/null
                if [ $? -ne 0 ]; then

                  cat $IMPLEMENTATION | grep 'HIirFilter' | grep -E "^ +HIIrFilter<T>(.+, probe).+" &> /dev/null
                  if [ $? -ne 0 ]; then

                    cat ../hardt/libhardt/include/$HEADER | grep 'HIirFilter' | grep -E "^ +HIirFilter<T>(.+, probe).+" &> /dev/null
                    if [ $? -ne 0 ]; then

                      cat $IMPLEMENTATION | grep 'HFileReader' | grep -E "^ +HFileReader<T>(.+, probe).+" &> /dev/null
                      if [ $? -ne 0 ]; then

                        cat ../hardt/libhardt/include/$HEADER | grep 'HFileReader' | grep -E "^ +HFileReader<T>(.+, probe).+" &> /dev/null
                        if [ $? -ne 0 ]; then

                          cat $IMPLEMENTATION | grep 'HInterpolator' | grep -E "^ +HInterpolator<T>(.+, probe).+" &> /dev/null
                          if [ $? -ne 0 ]; then

                            cat ../hardt/libhardt/include/$HEADER | grep 'HInterpolator' | grep -E "^ +HInterpolator<T>(.+, probe).+" &> /dev/null
                            if [ $? -ne 0 ]; then

                              cat $IMPLEMENTATION | grep 'HCombFilter' | grep -E "^ +HCombFilter<T>(.+, probe).+" &> /dev/null
                              if [ $? -ne 0 ]; then

                                cat ../hardt/libhardt/include/$HEADER | grep 'HCombFilter' | grep -E "^ +HCombFilter<T>(.+, probe).+" &> /dev/null
                                if [ $? -ne 0 ]; then


                                  cat $IMPLEMENTATION | grep 'HFilter' | grep -E "^ +HFilter<std::complex<T>>(.+, probe).+" &> /dev/null
                                  if [ $? -ne 0 ]; then

                                    cat ../hardt/libhardt/include/$HEADER | grep 'HFilter' | grep -E "^ +HFilter<std::complex<T>>(.+, probe).+" &> /dev/null
                                    if [ $? -ne 0 ]; then

                                      if [ $MISSING -eq 0 ]; then
                                        echo
                                        echo "Header: ../hardt/libhardt/include/$HEADER"
                                        echo "Classname: $CLASSNAME"
                                        echo "Implementation: $IMPLEMENTATION"
                                        echo "  Reason: No usage of probe"
                                        MISSING=1
                                      fi
                                    fi
                                  fi
                                fi
                              fi
                            fi
                          fi
                        fi
                      fi
                    fi
                  fi
                fi
              fi
            fi
          fi
        fi
      fi
    done

  fi

done

