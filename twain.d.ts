type TW_VERSION = {
    MajorNum: number
    MinorNum: number
    Language: number
    Country: number
    Info: string
}

type TW_IDENTITY = {
    Id: null | string
    Version: TW_VERSION
    ProtocolMajor: number
    ProtocolMinor: number
    SupportedGroups: number
    Manufacturer: string
    ProductFamily: string
    ProductName: string
}

interface Configure {

}


declare module "*twain" {
    const TWON_PROTOCOLMAJOR: number
    const TWON_PROTOCOLMINOR: number
    const DF_APP2: number

    declare class TwainApp {
        state: number = 1
        constructor(Configure)
        connectDSM: () => boolean
    }
}