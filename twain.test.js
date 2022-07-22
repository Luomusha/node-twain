const bindings = require('bindings')
const twain = bindings("twain")

describe('twain', () => {
    it('Protocol Version"', () => {
        expect(twain.TWON_PROTOCOLMAJOR).toBe(2)
        expect(twain.TWON_PROTOCOLMINOR).toBe(4)
    })

    it("Country Code & Language Code", () => {
        expect(twain.TWCY_CHINA).toBe(86)
        expect(twain.TWLG_CHINESE).toBe(37)
    })

    it('Twain class"', async () => {
        const app = new twain.TwainSDK( {
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
        expect(app.getState()).toBe(3)

        const sources = app.getDataSources()
        console.log(sources)
        expect(sources.length).toBeGreaterThan(0)

        const defaultSource = app.getDefaultSource()
        console.log(defaultSource);
        expect(defaultSource).toBeTruthy()

        app.setDefaultSource(sources[0])

        await app.openDataSource()
        const enumTest = app.getCapability(twain.ICAP_XFERMECH)         // Enum
        const oneValueTest = app.getCapability(twain.CAP_AUTHOR)  // one value
        const rangeTest = app.getCapability(twain.ICAP_JPEGQUALITY)        // range
        const arrayTest = app.getCapability(twain.CAP_SUPPORTEDCAPS)    // array
        expect(enumTest).toHaveProperty("currentIndex")
        expect(enumTest).toHaveProperty("defaultIndex")
        expect(enumTest).toHaveProperty("itemList")
        expect(rangeTest).toHaveProperty("minValue")
        expect(rangeTest).toHaveProperty("maxValue")
        expect(rangeTest).toHaveProperty("stepSize")
        expect(rangeTest).toHaveProperty("defaultValue")
        expect(rangeTest).toHaveProperty("currentValue")
        expect(Array.isArray(arrayTest)).toBeTruthy()
        expect(oneValueTest).toBeTruthy()

        app.setCallback()
        await app.enableDataSource()
        app.scan()
    })
})


