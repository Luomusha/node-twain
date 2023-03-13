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
        const session = new twain.TwainSDK({
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
        
        // pSource
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

        const c0 = session.getCapability(twain.CAP_XFERCOUNT)
        const c1 = session.getCapability(twain.ICAP_PIXELTYPE)
        const c2 = session.getCapability(twain.ICAP_XFERMECH)
        const c3 = session.getCapability(twain.ICAP_IMAGEFILEFORMAT)
        const c4 = session.getCapability(twain.ICAP_COMPRESSION)
        const c5 = session.getCapability(twain.ICAP_UNITS)
        const c6 = session.getCapability(twain.ICAP_BITDEPTH)
        const c7 = session.getCapability(twain.ICAP_XRESOLUTION)
        const c8 = session.getCapability(twain.ICAP_YRESOLUTION)
        const c9 = session.getCapability(twain.ICAP_FRAMES)

        console.log("CAP_XFERCOUNT", c0)
        console.log("ICAP_PIXELTYPE", c1)
        console.log("ICAP_XFERMECH", c2)
        console.log("ICAP_IMAGEFILEFORMAT", c3)
        console.log("ICAP_COMPRESSION", c4)
        console.log("ICAP_UNITS", c5)
        console.log("ICAP_BITDEPTH", c6)
        console.log("ICAP_XRESOLUTION", c7)
        console.log("ICAP_YRESOLUTION", c8)
        console.log("ICAP_FRAMES", c9)

        session.setCallback()
        await session.enableDataSource()
        session.scan(twain.TWSX_FILE, "C:\\Users\\A11200321050133\\Documents\\imageName")
    })
})


describe('quick', () => {

    it('quick class"', async () => {
        const session = new twain.TwainSDK({
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
        
        // sources
        // const sources = session.getDataSources()
        // expect(Array.isArray(sources)).toBeTruthy()
        // source
        const defaultSource = session.getDefaultSource()
        console.log(defaultSource)

        // pSource
        // session.setDefaultSource(defaultSource)

        // pSource, sources
        session.openDataSource(defaultSource)

        // session.openDataSource(defaultSource)
        // source
        session.setCallback()

        // pSource
        // session.enableDataSource()

        // pSource
        session.scan(twain.TWSX_FILE, "C:\\Users\\A11200321050133\\Documents\\Scanned Documents\\imageFromScanner")

        
        session.scan(twain.TWSX_FILE, "C:\\Users\\A11200321050133\\Documents\\Scanned Documents\\imageFromScanner2")
    })
})
