# Discrete HMM for Speech Recogition

In acoustic model,
- each word consists of syllables
- each syllable consists of phonemes
- each phoneme consists of some (hypothetical) states.

Each phoneme can be described by a HMM (acoustic model).
Each time frame, with an observance (MFCC vector) mapped to a state.
Hence, there are state transition probabilities a_ij and observation distribution b_j(o_t) in each phoneme acoustic model.

Usually in speech recognition we restrict the HMM to be a left-to-right model, and the observation distribution are assumed to be a continuous Gaussian mixture model. Here we use characters to simulate the observance.

### training HMM
- Algorithm: Baum-Welch
- input:
  - observed sequences (seq_model_01~05.txt)
  - initial state & transition probabilities (model_init.txt)
  - number of iteration
- output:
  - trained models for 5 sequences

### testing HMM
- Algorithm: Viterbi
- input: 
  - trained model
  - testing sequences 
  - modellist.txt 
- output
  - best labels (with the higest probability among the five models)
  
### Experiment
see the document
