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

const app = new twain.TwainSession( {
    productName: "productName!",
    productFamily: "productFamily!",
    manufacturer: "manufacturer!",
    version: {
        country: twain.TWCY_CHINA,
        language: twain.TWLG_CHINESE,
        majorNum: 1,
        minorNum: 1,
        info: "v0.0.1",
    }
})
const sources = app.getDataSources()            // ["PaperStream IP SP-1120N #2"]
const defaultSource = app.getDefaultSource()    // "PaperStream IP SP-1120N #2"
app.setDefaultSource(sources[0])

await app.openDataSource()

const enumTest = app.getCapability(twain.ICAP_XFERMECH)         // Enum
const oneValueTest = app.getCapability(twain.CAP_AUTHOR)        // one value
const rangeTest = app.getCapability(twain.ICAP_JPEGQUALITY)     // range
const arrayTest = app.getCapability(twain.CAP_SUPPORTEDCAPS)    // array

app.scan();
```

## Install

```bash
npm i node-twain
```

## License
MIT License. See the LICENSE file.