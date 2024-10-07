// 0x6080604052348015600e575f80fd5b5060a580601a5f395ff3fe6080604052348015600e575f80fd5b50600436106030575f3560e01c8063cdfead2e146034578063e026c017146045575b5f80fd5b6043603f3660046059565b5f55565b005b5f5460405190815260200160405180910390f35b5f602082840312156068575f80fd5b503591905056fea2646970667358221220ab527e4ef269992bda5b7037bd813f48d9f4552eae340f70d3cb35c1b95dceee64736f6c634300081a0033

// 3 Sections
// 1. Contract Creation Code
// 2. Runtime Bytecode
// 3. Metadata

// Stores value at free memory location
PUSH1 0x80        // top [0x80] bottom
PUSH1 0x40        // [0x40, 0x80]
MSTORE            // [] - stores 0x80 to the memory location at 0x40

// Jumps to location 0x0e if no eth is sent with the calldata
CALLVALUE         // [msg.value] - checks if there is a value sent with the transaction
DUP1              // [msg.value, msg.value] - duplicates top of the stack
ISZERO            // [msg.value == 0, msg.value] - checks if top of stack is zero
PUSH1 0x0e        // [0x0e, msg.value == 0, msg.value] - 0x0e is actually the program counter (PC) for JUMPDEST
JUMPI             // [msg.value]

// If not (people sent eth), reverts without a message
PUSH0            // [0x00, msg.value]
DUP1             // [0x00, 0x00, msg.value] - isn't it better to PUSH0 (2 gas) instead of DUP1 (3 gas)?
REVERT           // [msg.value]

// Jump dest if msg.value == 0
// Sticks runtime code on chain
JUMPDEST        // [msg.value]
POP             // []
PUSH1 0xa5      // [0xa5]
DUP1            // [0xa5, 0xa5]
PUSH2 0x001b    // [0x001b, 0xa5, 0xa5]
PUSH0          // [0x00, 0x001b, 0xa5, 0xa5]        - copy the 27th opcode all the way till 165 opcodes later. And paste it in the 0th location of the memory
CODECOPY       // [0xa5]                            Memory: [runtime code]
PUSH0          // [0x00, 0xa5]
RETURN         // [] - copy 165 opcodes from 0x00 location in the memory and return it
INVALID        // [] - signals end of contract creation code

// Code that is stuck on chain
// Entry point for all calls
// Free memory pointer
PUSH1 0x80
PUSH1 0x40
MSTORE

// Check if any eth is sent with the call, if yes, revert
CALLVALUE       // [msg.value]
DUP1            // [msg.value, msg.value]
ISZERO          // [msg.value == 0, msg.value]
PUSH1 0x0e      // [0x0e, msg.value == 0, msg.value]
JUMPI           // [msg.value]
PUSH0           // [0x00, msg.value]
DUP1            // [0x00, 0x00, msg.value]
REVERT          // [msg.value]

// If no eth is sent, continue here
// Checking to see if there is enough bytes in calldata for a function selector
JUMPDEST        // [msg.value]
POP             // []
PUSH1 0x04      // [0x04]
CALLDATASIZE    // [calldata_size, 0x04]
LT              // [calldata_size < 0x04]
PUSH1 0x30      // [0x30, calldata_size < 0x04]
JUMPI
// if calldata_size < 0x04, go to calldata_jump

// function dispatcher in solidity
PUSH0           // [0x00]
CALLDATALOAD    // [32 bytes of calldata]
PUSH1 0xe0      // [0xe0, 32 bytes of calldata]
SHR             // [calldata[0:4]] // function selector //  - shr shifts calldata to the right and pads the left part with 0s

// Dispatching for setNumberOfHorses
DUP1            // [function selector, function selector]
PUSH4 0xcdfead2e // [0xcdfead2e, function selector, function selector]
EQ               // [0xcdfead2e == function selector, function selector]
PUSH1 0x34       // [0x34, 0xcdfead2e == function selector, function selector]
JUMPI            // [function selector]
// if func_selector == 0xcdfead2e -> set_num_of_horses

// Dispatching for getNumberOfHorses
DUP1             // [function selector, function selector]
PUSH4 0xe026c017 // [0xe026c017, function selector, function selector]
EQ               // [0xe026c017 == function selector, function selector]
PUSH1 0x45       // [0x45, 0xe026c017 == function selector, function selector]
JUMPI            // [function selector]
// if func_selector == 0xe026c017 -> get_num_of_horses

// Calldata jump
// Revert Jumpdest
JUMPDEST    // []
PUSH0       // [0]
DUP1        // [0, 0]
REVERT      // [0]

// updateHorseNumber jump dest 1
// Setup jumping program counters in the stack
JUMPDEST        // [function selector]
PUSH1 0x43      // [0x43, function selector]
PUSH1 0x3f      // [0x3f, 0x43, function selector]
CALLDATASIZE    // [calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x04      // [0x04, calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x59      // [0x59, 0x04, calldata_size, 0x3f, 0x43, function selector]
JUMP            // [0x04, calldata_size, 0x3f, 0x43, function selector]

// Jump dest 4
// We can finally run an sstore to save our value to storage
// 1. Function dispatch
// 2. Checked for msg.value
// 3. Checked for calldata size
// 4. Received the number to use from the calldata
JUMPDEST        // [calldata(of numberToUpdate), 0x43, function selector]
PUSH0           // [0x00, calldata(of numberToUpdate), 0x43, function selector]
SSTORE          // [0x43, function selector]
JUMP            // [function selector]
// Jump dest 5

// Jump dest 5
JUMPDEST        // [function selector]
STOP            // // [function selector]  
JUMPDEST
PUSH0
SLOAD
PUSH1 0x40
MLOAD
SWAP1
DUP2
MSTORE
PUSH1 0x20
ADD
PUSH1 0x40
MLOAD
DUP1
SWAP2
SUB
SWAP1
RETURN

// updateHorseNumber jump dest 2
JUMPDEST        // [0x04, calldata_size, 0x3f, 0x43, func_selector]
PUSH0           // [0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x20      // [0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
DUP3            // [0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
DUP5            // [calldata_size, 0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
SUB             // [calldata_size - 0x04, 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
// is there more calldata than just the function selector?
SLT             // [calldata_size - 0x04 < 0x20, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
ISZERO          // [more_calldata_than_selector?, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
PUSH1 0x68      // [0x68, more_calldata_than_selector?, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
JUMPI           // [0, 0x04, calldata_size, 0x3f, 0x43, function selector]
// We are going to jump to jump dest 3 if there is more calldata than:
// function selector + 0x20

// Revert if there isn't enough calldata!
PUSH0           // [0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
DUP1            // [0x00, 0x00, 0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]
REVERT          // [0x00, 0x04, calldata_size, 0x3f, 0x43, function selector]

// updateHorseNumber jump dest 3
// Grab the calldata for updating the horse number
// Delete some stuff in the stack
JUMPDEST        // [0, 0x04, calldata_size, 0x3f, 0x43, function selector]
POP             // [0x04, calldata_size, 0x3f, 0x43, function selector]
// Ignore the function selector, and just grab the data
// 0xcdfead2e 0000000000000000000000000000000000000000000000000000000000000007
CALLDATALOAD    // [calldata(of numberToUpdate), calldata_size, 0x3f, 0x43, function selector]
SWAP2           // [0x3f, calldata_size, calldata(of numberToUpdate), 0x43, function selector]
SWAP1           // [calldata_size, 0x3f, calldata(of numberToUpdate), 0x43, function selector]
POP             // [0x3f, calldata(of numberToUpdate), 0x43, function selector]]
JUMP
INVALID
LOG2
PUSH5 0x6970667358
INVALID
SLT
KECCAK256
INVALID
MSTORE
PUSH31 0x4ef269992bda5b7037bd813f48d9f4552eae340f70d3cb35c1b95dceee6473
PUSH16 0x6c634300081a0033