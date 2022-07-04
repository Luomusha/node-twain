# node-twain

A nodejs library for TWAIN using node-addon-api.

NOTE: Project is under develop.

## Installation
``$ npm install node-twain``

## Usage

```typescript
import {Twain} from "node-twain"

const twain = new Twain()

twain.connectDSM()  // state 1 -> state 2 -> 3
twain.listDS()      // state 3
twain.connectDS()   // state 3 -> state 4
twain.enableDS()

```

## License
MIT License. See the LICENSE file.