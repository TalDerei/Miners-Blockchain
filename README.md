# CSE297 Team Miners Repository 
This is the "Team Miners" repository for CSE 297 (Fall 2020).

## Branches
'git checkout phase-1' for Phase-1 Branch  
'git checkout phase-2' for Phase-2 Branch  
'git checkout phase-3' for Phase-3 Branch  

## Set-Up
To get it running, go into /cse297_team_miners/src directory and run two-shell scripts:
`bash recompile` to clean and recompile the code-base with make utility
`bash run` to execute the executable

Create a .txt file (or multiple .txt files) within the same directory, and the program will construct
merkle-trees and a cryptographically-secure (SHA-256 based) blockchain containing the file contents.

## Phase 1 and 2
The code-base currently contains the completed code necessary to satisfy the requirments for phases 1
and 2. 

`Phase 1:` Construct Merkle/Patricia tree over a set of strings from a user-defined input file.
`Phase 2:` Consruct a blockchain containing Merkle/Patricia trees and header contents in a linked-list structure.

*** IGNORE THE TYPE-CASTING WARNINGS FOR NOW -- PHASE-3 IN-PROGRESS ***

# Contributors
- Tal Derei <tad222@lehigh.edu>
- Xinhao Liu <xil222@lehigh.edu>
- Yifan She <yis223@lehigh.edu>
- Chengshuo Huang <chh221@lehigh.edu>
- Haoluo Fu <haf222@lehigh.edu>
