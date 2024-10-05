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
PUSH1 0x40
MSTORE
CALLVALUE
DUP1
ISZERO
PUSH1 0x0e
JUMPI
PUSH0
DUP1
REVERT
JUMPDEST
POP
PUSH1 0x04
CALLDATASIZE
LT
PUSH1 0x30
JUMPI
PUSH0
CALLDATALOAD
PUSH1 0xe0
SHR
DUP1
PUSH4 0xcdfead2e
EQ
PUSH1 0x34
JUMPI
DUP1
PUSH4 0xe026c017
EQ
PUSH1 0x45
JUMPI
JUMPDEST
PUSH0
DUP1
REVERT
JUMPDEST
PUSH1 0x43
PUSH1 0x3f
CALLDATASIZE
PUSH1 0x04
PUSH1 0x59
JUMP
JUMPDEST
PUSH0
SSTORE
JUMP
JUMPDEST
STOP
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
JUMPDEST
PUSH0
PUSH1 0x20
DUP3
DUP5
SUB
SLT
ISZERO
PUSH1 0x68
JUMPI
PUSH0
DUP1
REVERT
JUMPDEST
POP
CALLDATALOAD
SWAP2
SWAP1
POP
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