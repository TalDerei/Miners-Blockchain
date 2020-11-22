# CSE297 Team Miners Repository 
This is the "Team Miners" repository for CSE 297 (Fall 2020).

## Branches
'git checkout phase-1' for Phase-1 Branch  
'git checkout phase-2' for Phase-2 Branch  
'git checkout phase-3' for Phase-3 Branch  

## Set-Up
To get it running, go into `/cse297_team_miners/src` directory and run two-shell scripts:
`bash recompile` to clean and recompile the code-base with make utility
`bash run` to execute the executable

Create a .txt file (or multiple text files) within the same directory, and the program will construct
merkle-trees and a cryptographically-secure (SHA-256 based) blockchain containing the file contents.

## Phase 1 and 2
The code-base currently contains the completed code necessary to satisfy the requirments for phases 1
and 2. Phase 3 in-progress. 

`Phase 1:`  Construct Merkle/Patricia tree over a set of strings from a user-defined input file.
`Phase 2:`  Consruct a blockchain containing Merkle/Patricia trees and header contents in a linked-list structure.
`Phase 3: ` Validation of the blockchain and proof of membership/non-membership.

## Text Files (.txt)
We've included two standard text-files, `input.txt` and `input2.txt` respectively, to serve as standard references. 
Each input file will output 3 SEPERATE files: (1) `name_of_file.out.txt` containing the merkle tree, (2) `name_of_file.block.txt`
containing the block contents (merkle tree and block header), and (3) `serialization.blockchain.txt` containing the serialized
data (residing on disk) associated with all the files inputted. 

*** IGNORE THE TYPE-CASTING WARNINGS FOR NOW ***

# Contributors
- Tal Derei <tad222@lehigh.edu>
- Xinhao Liu <xil222@lehigh.edu>
- Yifan She <yis223@lehigh.edu>
- Chengshuo Huang <chh221@lehigh.edu>
- Haoluo Fu <haf222@lehigh.edu>