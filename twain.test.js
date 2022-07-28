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
        const session = new twain.TwainSDK( {
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
        expect(session.getState()).toBe(3)

        const sources = session.getDataSources()
        console.log(sources)
        expect(sources.length).toBeGreaterThan(0)

        const defaultSource = session.getDefaultSource()
        console.log(defaultSource);
        expect(defaultSource).toBeTruthy()

        session.setDefaultSource(sources[0])

        await session.openDataSource()
        const enumTest = session.getCapability(twain.ICAP_XFERMECH)         // Enum
        const oneValueTest = session.getCapability(twain.CAP_AUTHOR)  // one value
        const rangeTest = session.getCapability(twain.ICAP_JPEGQUALITY)        // range
        const arrayTest = session.getCapability(twain.CAP_SUPPORTEDCAPS)    // array
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

        session.setCallback()
        await session.enableDataSource()
        session.scan(twain.TWSX_FILE)
    })

    it('dev test"', async () => {
        const session = new twain.TwainSDK( {
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
        console.log("999999")
        await session.openDataSource("dddd")
        console.log("HHHHHHHH")
    })
})


