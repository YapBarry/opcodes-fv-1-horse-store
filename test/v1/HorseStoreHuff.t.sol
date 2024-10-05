// SPDX-License Identifier: GPL-3.0-only
pragma solidity 0.8.26;

import {Base_TestV1, HorseStore} from "./Base_TestV1.t.sol";
import {HuffDeployer} from "foundry-huff/HuffDeployer.sol";

contract HorseStoreSolc is Base_TestV1 {
    string public constant HORSE_STORE_HUFF_LOCATION = "horseStoreV1/HorseStore"; // assumes the location is in src automatically
    function setUp() public override{
        // horseStore = // Huff Edition
        horseStore = HorseStore(HuffDeployer.config().deploy(HORSE_STORE_HUFF_LOCATION));
    }
}