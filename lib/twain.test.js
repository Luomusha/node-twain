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

    it('Twain class"', () => {
        const app = new twain.TwainApp()
        expect(app.connectDSM()).toBe(true)
    })
})

