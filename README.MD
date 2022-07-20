# node-twain

A nodejs library for TWAIN using node-addon-api.

NOTE: Project is under develop.

## Installation
``$ npm install node-twain``

## Twain flow
![img.png](assets/img.png)

## Usage

```typescript
import {Twain} from "node-twain"

const twain = new Twain()               // state 1 -> state 2
twain.openDataSourceManager()           // state 2 -> state 3
const list = twain.getDataSources()     // state 3
twain.openDataSource(ds)                // state 3 -> state 4
twain.enableDataSource(ds)              // state 4 -> state 5

twain.closeDataSourceManager()
```

## License
MIT License. See the LICENSE file.