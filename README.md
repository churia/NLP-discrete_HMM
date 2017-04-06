# Discrete HMM for Speech Recogition

In acoustic model,
- each word consists of syllables
- each syllable consists of phonemes
- each phoneme consists of some (hypothetical) states.

Each phoneme can be described by a HMM (acoustic model).
Each time frame, with an observance (MFCC vector) mapped to a state. 
Hence, there are state transition probabilities a_ij and observation distribution b_j(o_t) in each phoneme acoustic model.

Usually in speech recognition we restrict the HMM to be a left-to-right model, and the observation distribution are assumed to be a continuous Gaussian mixture model.

### training HMM
Baum-Welch

### testing HMM
Viterbi
