type Version = {
    majorNum: number
    minorNum: number
    language: number
    country: number
    info: string
}

type Identity = {
    version: Version
    manufacturer: string
    productFamily: string
    productName: string
}

type CAP_ARRAY = string;

type CAP_VALUE = Number[]

type CAP_ENUM = {
    currentIndex: number;
    defaultIndex: number;
    itemList: Number[];
}

type CAP_RANGE = {
    minValue: number;
    maxvalue: number;
    stepSize: number;
    defaultValue: number;
    currentValue: number;
}

type Capability = CAP_RANGE | CAP_ENUM | CAP_VALUE | CAP_ARRAY;


interface Configure {
    productName: string
}


declare module "node-twain" {

    /****************************************************************************
     * Country Constants                                                        *
     ****************************************************************************/

    const TWCY_AFGHANISTAN: number
    const TWCY_ALGERIA: number
    const TWCY_AMERICANSAMOA: number
    const TWCY_ANDORRA: number
    const TWCY_ANGOLA: number
    const TWCY_ANGUILLA: number
    const TWCY_ANTIGUA: number
    const TWCY_ARGENTINA: number
    const TWCY_ARUBA: number
    const TWCY_ASCENSIONI: number
    const TWCY_AUSTRALIA: number
    const TWCY_AUSTRIA: number
    const TWCY_BAHAMAS: number
    const TWCY_BAHRAIN: number
    const TWCY_BANGLADESH: number
    const TWCY_BARBADOS: number
    const TWCY_BELGIUM: number
    const TWCY_BELIZE: number
    const TWCY_BENIN: number
    const TWCY_BERMUDA: number
    const TWCY_BHUTAN: number
    const TWCY_BOLIVIA: number
    const TWCY_BOTSWANA: number
    const TWCY_BRITAIN: number
    const TWCY_BRITVIRGINIS: number
    const TWCY_BRAZIL: number
    const TWCY_BRUNEI: number
    const TWCY_BULGARIA: number
    const TWCY_BURKINAFASO: number
    const TWCY_BURMA: number
    const TWCY_BURUNDI: number
    const TWCY_CAMAROON: number
    const TWCY_CANADA: number
    const TWCY_CAPEVERDEIS: number
    const TWCY_CAYMANIS: number
    const TWCY_CENTRALAFREP: number
    const TWCY_CHAD: number
    const TWCY_CHILE: number
    const TWCY_CHINA: number
    const TWCY_CHRISTMASIS: number
    const TWCY_COCOSIS: number
    const TWCY_COLOMBIA: number
    const TWCY_COMOROS: number
    const TWCY_CONGO: number
    const TWCY_COOKIS: number
    const TWCY_COSTARICA: number
    const TWCY_CUBA: number
    const TWCY_CYPRUS: number
    const TWCY_CZECHOSLOVAKIA: number
    const TWCY_DENMARK: number
    const TWCY_DJIBOUTI: number
    const TWCY_DOMINICA: number
    const TWCY_DOMINCANREP: number
    const TWCY_EASTERIS: number
    const TWCY_ECUADOR: number
    const TWCY_EGYPT: number
    const TWCY_ELSALVADOR: number
    const TWCY_EQGUINEA: number
    const TWCY_ETHIOPIA: number
    const TWCY_FALKLANDIS: number
    const TWCY_FAEROEIS: number
    const TWCY_FIJIISLANDS: number
    const TWCY_FINLAND: number
    const TWCY_FRANCE: number
    const TWCY_FRANTILLES: number
    const TWCY_FRGUIANA: number
    const TWCY_FRPOLYNEISA: number
    const TWCY_FUTANAIS: number
    const TWCY_GABON: number
    const TWCY_GAMBIA: number
    const TWCY_GERMANY: number
    const TWCY_GHANA: number
    const TWCY_GIBRALTER: number
    const TWCY_GREECE: number
    const TWCY_GREENLAND: number
    const TWCY_GRENADA: number
    const TWCY_GRENEDINES: number
    const TWCY_GUADELOUPE: number
    const TWCY_GUAM: number
    const TWCY_GUANTANAMOBAY: number
    const TWCY_GUATEMALA: number
    const TWCY_GUINEA: number
    const TWCY_GUINEABISSAU: number
    const TWCY_GUYANA: number
    const TWCY_HAITI: number
    const TWCY_HONDURAS: number
    const TWCY_HONGKONG: number
    const TWCY_HUNGARY: number
    const TWCY_ICELAND: number
    const TWCY_INDIA: number
    const TWCY_INDONESIA: number
    const TWCY_IRAN: number
    const TWCY_IRAQ: number
    const TWCY_IRELAND: number
    const TWCY_ISRAEL: number
    const TWCY_ITALY: number
    const TWCY_IVORYCOAST: number
    const TWCY_JAMAICA: number
    const TWCY_JAPAN: number
    const TWCY_JORDAN: number
    const TWCY_KENYA: number
    const TWCY_KIRIBATI: number
    const TWCY_KOREA: number
    const TWCY_KUWAIT: number
    const TWCY_LAOS: number
    const TWCY_LEBANON: number
    const TWCY_LIBERIA: number
    const TWCY_LIBYA: number
    const TWCY_LIECHTENSTEIN: number
    const TWCY_LUXENBOURG: number
    const TWCY_MACAO: number
    const TWCY_MADAGASCAR: number
    const TWCY_MALAWI: number
    const TWCY_MALAYSIA: number
    const TWCY_MALDIVES: number
    const TWCY_MALI: number
    const TWCY_MALTA: number
    const TWCY_MARSHALLIS: number
    const TWCY_MAURITANIA: number
    const TWCY_MAURITIUS: number
    const TWCY_MEXICO: number
    const TWCY_MICRONESIA: number
    const TWCY_MIQUELON: number
    const TWCY_MONACO: number
    const TWCY_MONGOLIA: number
    const TWCY_MONTSERRAT: number
    const TWCY_MOROCCO: number
    const TWCY_MOZAMBIQUE: number
    const TWCY_NAMIBIA: number
    const TWCY_NAURU: number
    const TWCY_NEPAL: number
    const TWCY_NETHERLANDS: number
    const TWCY_NETHANTILLES: number
    const TWCY_NEVIS: number
    const TWCY_NEWCALEDONIA: number
    const TWCY_NEWZEALAND: number
    const TWCY_NICARAGUA: number
    const TWCY_NIGER: number
    const TWCY_NIGERIA: number
    const TWCY_NIUE: number
    const TWCY_NORFOLKI: number
    const TWCY_NORWAY: number
    const TWCY_OMAN: number
    const TWCY_PAKISTAN: number
    const TWCY_PALAU: number
    const TWCY_PANAMA: number
    const TWCY_PARAGUAY: number
    const TWCY_PERU: number
    const TWCY_PHILLIPPINES: number
    const TWCY_PITCAIRNIS: number
    const TWCY_PNEWGUINEA: number
    const TWCY_POLAND: number
    const TWCY_PORTUGAL: number
    const TWCY_QATAR: number
    const TWCY_REUNIONI: number
    const TWCY_ROMANIA: number
    const TWCY_RWANDA: number
    const TWCY_SAIPAN: number
    const TWCY_SANMARINO: number
    const TWCY_SAOTOME: number
    const TWCY_SAUDIARABIA: number
    const TWCY_SENEGAL: number
    const TWCY_SEYCHELLESIS: number
    const TWCY_SIERRALEONE: number
    const TWCY_SINGAPORE: number
    const TWCY_SOLOMONIS: number
    const TWCY_SOMALI: number
    const TWCY_SOUTHAFRICA: number
    const TWCY_SPAIN: number
    const TWCY_SRILANKA: number
    const TWCY_STHELENA: number
    const TWCY_STKITTS: number
    const TWCY_STLUCIA: number
    const TWCY_STPIERRE: number
    const TWCY_STVINCENT: number
    const TWCY_SUDAN: number
    const TWCY_SURINAME: number
    const TWCY_SWAZILAND: number
    const TWCY_SWEDEN: number
    const TWCY_SWITZERLAND: number
    const TWCY_SYRIA: number
    const TWCY_TAIWAN: number
    const TWCY_TANZANIA: number
    const TWCY_THAILAND: number
    const TWCY_TOBAGO: number
    const TWCY_TOGO: number
    const TWCY_TONGAIS: number
    const TWCY_TRINIDAD: number
    const TWCY_TUNISIA: number
    const TWCY_TURKEY: number
    const TWCY_TURKSCAICOS: number
    const TWCY_TUVALU: number
    const TWCY_UGANDA: number
    const TWCY_USSR: number
    const TWCY_UAEMIRATES: number
    const TWCY_UNITEDKINGDOM: number
    const TWCY_USA: number
    const TWCY_URUGUAY: number
    const TWCY_VANUATU: number
    const TWCY_VATICANCITY: number
    const TWCY_VENEZUELA: number
    const TWCY_WAKE: number
    const TWCY_WALLISIS: number
    const TWCY_WESTERNSAHARA: number
    const TWCY_WESTERNSAMOA: number
    const TWCY_YEMEN: number
    const TWCY_YUGOSLAVIA: number
    const TWCY_ZAIRE: number
    const TWCY_ZAMBIA: number
    const TWCY_ZIMBABWE: number
    const TWCY_ALBANIA: number
    const TWCY_ARMENIA: number
    const TWCY_AZERBAIJAN: number
    const TWCY_BELARUS: number
    const TWCY_BOSNIAHERZGO: number
    const TWCY_CAMBODIA: number
    const TWCY_CROATIA: number
    const TWCY_CZECHREPUBLIC: number
    const TWCY_DIEGOGARCIA: number
    const TWCY_ERITREA: number
    const TWCY_ESTONIA: number
    const TWCY_GEORGIA: number
    const TWCY_LATVIA: number
    const TWCY_LESOTHO: number
    const TWCY_LITHUANIA: number
    const TWCY_MACEDONIA: number
    const TWCY_MAYOTTEIS: number
    const TWCY_MOLDOVA: number
    const TWCY_MYANMAR: number
    const TWCY_NORTHKOREA: number
    const TWCY_PUERTORICO: number
    const TWCY_RUSSIA: number
    const TWCY_SERBIA: number
    const TWCY_SLOVAKIA: number
    const TWCY_SLOVENIA: number
    const TWCY_SOUTHKOREA: number
    const TWCY_UKRAINE: number
    const TWCY_USVIRGINIS: number
    const TWCY_VIETNAM: number

    /****************************************************************************
     * Language Constants                                                       *
     ****************************************************************************/
    const TWLG_USERLOCALE: number
    const TWLG_DAN: number
    const TWLG_DUT: number
    const TWLG_ENG: number
    const TWLG_FCF: number
    const TWLG_FIN: number
    const TWLG_FRN: number
    const TWLG_GER: number
    const TWLG_ICE: number
    const TWLG_ITN: number
    const TWLG_NOR: number
    const TWLG_POR: number
    const TWLG_SPA: number
    const TWLG_SWE: number
    const TWLG_USA: number
    const TWLG_AFRIKAANS: number
    const TWLG_ALBANIA: number
    const TWLG_ARABIC: number
    const TWLG_ARABIC_ALGERIA: number
    const TWLG_ARABIC_BAHRAIN: number
    const TWLG_ARABIC_EGYPT: number
    const TWLG_ARABIC_IRAQ: number
    const TWLG_ARABIC_JORDAN: number
    const TWLG_ARABIC_KUWAIT: number
    const TWLG_ARABIC_LEBANON: number
    const TWLG_ARABIC_LIBYA: number
    const TWLG_ARABIC_MOROCCO: number
    const TWLG_ARABIC_OMAN: number
    const TWLG_ARABIC_QATAR: number
    const TWLG_ARABIC_SAUDIARABIA: number
    const TWLG_ARABIC_SYRIA: number
    const TWLG_ARABIC_TUNISIA: number
    const TWLG_ARABIC_UAE: number
    const TWLG_ARABIC_YEMEN: number
    const TWLG_BASQUE: number
    const TWLG_BYELORUSSIAN: number
    const TWLG_BULGARIAN: number
    const TWLG_CATALAN: number
    const TWLG_CHINESE: number
    const TWLG_CHINESE_HONGKONG: number
    const TWLG_CHINESE_PRC: number
    const TWLG_CHINESE_SINGAPORE: number
    const TWLG_CHINESE_SIMPLIFIED: number
    const TWLG_CHINESE_TAIWAN: number
    const TWLG_CHINESE_TRADITIONAL: number
    const TWLG_CROATIA: number
    const TWLG_CZECH: number
    const TWLG_DANISH: number
    const TWLG_DUTCH: number
    const TWLG_DUTCH_BELGIAN: number
    const TWLG_ENGLISH: number
    const TWLG_ENGLISH_AUSTRALIAN: number
    const TWLG_ENGLISH_CANADIAN: number
    const TWLG_ENGLISH_IRELAND: number
    const TWLG_ENGLISH_NEWZEALAND: number
    const TWLG_ENGLISH_SOUTHAFRICA: number
    const TWLG_ENGLISH_UK: number
    const TWLG_ENGLISH_USA: number
    const TWLG_ESTONIAN: number
    const TWLG_FAEROESE: number
    const TWLG_FARSI: number
    const TWLG_FINNISH: number
    const TWLG_FRENCH: number
    const TWLG_FRENCH_BELGIAN: number
    const TWLG_FRENCH_CANADIAN: number
    const TWLG_FRENCH_LUXEMBOURG: number
    const TWLG_FRENCH_SWISS: number
    const TWLG_GERMAN: number
    const TWLG_GERMAN_AUSTRIAN: number
    const TWLG_GERMAN_LUXEMBOURG: number
    const TWLG_GERMAN_LIECHTENSTEIN: number
    const TWLG_GERMAN_SWISS: number
    const TWLG_GREEK: number
    const TWLG_HEBREW: number
    const TWLG_HUNGARIAN: number
    const TWLG_ICELANDIC: number
    const TWLG_INDONESIAN: number
    const TWLG_ITALIAN: number
    const TWLG_ITALIAN_SWISS: number
    const TWLG_JAPANESE: number
    const TWLG_KOREAN: number
    const TWLG_KOREAN_JOHAB: number
    const TWLG_LATVIAN: number
    const TWLG_LITHUANIAN: number
    const TWLG_NORWEGIAN: number
    const TWLG_NORWEGIAN_BOKMAL: number
    const TWLG_NORWEGIAN_NYNORSK: number
    const TWLG_POLISH: number
    const TWLG_PORTUGUESE: number
    const TWLG_PORTUGUESE_BRAZIL: number
    const TWLG_ROMANIAN: number
    const TWLG_RUSSIAN: number
    const TWLG_SERBIAN_LATIN: number
    const TWLG_SLOVAK: number
    const TWLG_SLOVENIAN: number
    const TWLG_SPANISH: number
    const TWLG_SPANISH_MEXICAN: number
    const TWLG_SPANISH_MODERN: number
    const TWLG_SWEDISH: number
    const TWLG_THAI: number
    const TWLG_TURKISH: number
    const TWLG_UKRANIAN: number
    const TWLG_ASSAMESE: number
    const TWLG_BENGALI: number
    const TWLG_BIHARI: number
    const TWLG_BODO: number
    const TWLG_DOGRI: number
    const TWLG_GUJARATI: number
    const TWLG_HARYANVI: number
    const TWLG_HINDI: number
    const TWLG_KANNADA: number
    const TWLG_KASHMIRI: number
    const TWLG_MALAYALAM: number
    const TWLG_MARATHI: number
    const TWLG_MARWARI: number
    const TWLG_MEGHALAYAN: number
    const TWLG_MIZO: number
    const TWLG_NAGA: number
    const TWLG_ORISSI: number
    const TWLG_PUNJABI: number
    const TWLG_PUSHTU: number
    const TWLG_SERBIAN_CYRILLIC: number
    const TWLG_SIKKIMI: number
    const TWLG_SWEDISH_FINLAND: number
    const TWLG_TAMIL: number
    const TWLG_TELUGU: number
    const TWLG_TRIPURI: number
    const TWLG_URDU: number
    const TWLG_VIETNAMESE: number

    const TWON_PROTOCOLMAJOR: number
    const TWON_PROTOCOLMINOR: number
    const DF_APP2: number

    const CAP_CUSTOMBASE: number
    const CAP_XFERCOUNT: number
    const ICAP_COMPRESSION: number
    const ICAP_PIXELTYPE: number
    const ICAP_UNITS: number
    const ICAP_XFERMECH: number
    const CAP_AUTHOR: number
    const CAP_CAPTION: number
    const CAP_FEEDERENABLED: number
    const CAP_FEEDERLOADED: number
    const CAP_TIMEDATE: number
    const CAP_SUPPORTEDCAPS: number
    const CAP_EXTENDEDCAPS: number
    const CAP_AUTOFEED: number
    const CAP_CLEARPAGE: number
    const CAP_FEEDPAGE: number
    const CAP_REWINDPAGE: number
    const CAP_INDICATORS: number
    const CAP_PAPERDETECTABLE: number
    const CAP_UICONTROLLABLE: number
    const CAP_DEVICEONLINE: number
    const CAP_AUTOSCAN: number
    const CAP_THUMBNAILSENABLED: number
    const CAP_DUPLEX: number
    const CAP_DUPLEXENABLED: number
    const CAP_ENABLEDSUIONLY: number
    const CAP_CUSTOMDSDATA: number
    const CAP_ENDORSER: number
    const CAP_JOBCONTROL: number
    const CAP_ALARMS: number
    const CAP_ALARMVOLUME: number
    const CAP_AUTOMATICCAPTURE: number
    const CAP_TIMEBEFOREFIRSTCAPTURE: number
    const CAP_TIMEBETWEENCAPTURES: number
    const CAP_MAXBATCHBUFFERS: number
    const CAP_DEVICETIMEDATE: number
    const CAP_POWERSUPPLY: number
    const CAP_CAMERAPREVIEWUI: number
    const CAP_DEVICEEVENT: number
    const CAP_SERIALNUMBER: number
    const CAP_PRINTER: number
    const CAP_PRINTERENABLED: number
    const CAP_PRINTERINDEX: number
    const CAP_PRINTERMODE: number
    const CAP_PRINTERSTRING: number
    const CAP_PRINTERSUFFIX: number
    const CAP_LANGUAGE: number
    const CAP_FEEDERALIGNMENT: number
    const CAP_FEEDERORDER: number
    const CAP_REACQUIREALLOWED: number
    const CAP_BATTERYMINUTES: number
    const CAP_BATTERYPERCENTAGE: number
    const CAP_CAMERASIDE: number
    const CAP_SEGMENTED: number
    const CAP_CAMERAENABLED: number
    const CAP_CAMERAORDER: number
    const CAP_MICRENABLED: number
    const CAP_FEEDERPREP: number
    const CAP_FEEDERPOCKET: number
    const CAP_AUTOMATICSENSEMEDIUM: number
    const CAP_CUSTOMINTERFACEGUID: number
    const CAP_SUPPORTEDCAPSSEGMENTUNIQUE: number
    const CAP_SUPPORTEDDATS: number
    const CAP_DOUBLEFEEDDETECTION: number
    const CAP_DOUBLEFEEDDETECTIONLENGTH: number
    const CAP_DOUBLEFEEDDETECTIONSENSITIVITY: number
    const CAP_DOUBLEFEEDDETECTIONRESPONSE: number
    const CAP_PAPERHANDLING: number
    const CAP_INDICATORSMODE: number
    const CAP_PRINTERVERTICALOFFSET: number
    const CAP_POWERSAVETIME: number
    const CAP_PRINTERCHARROTATION: number
    const CAP_PRINTERFONTSTYLE: number
    const CAP_PRINTERINDEXLEADCHAR: number
    const CAP_PRINTERINDEXMAXVALUE: number
    const CAP_PRINTERINDEXNUMDIGITS: number
    const CAP_PRINTERINDEXSTEP: number
    const CAP_PRINTERINDEXTRIGGER: number
    const CAP_PRINTERSTRINGPREVIEW: number
    const CAP_SHEETCOUNT: number
    const ICAP_AUTOBRIGHT: number
    const ICAP_BRIGHTNESS: number
    const ICAP_CONTRAST: number
    const ICAP_CUSTHALFTONE: number
    const ICAP_EXPOSURETIME: number
    const ICAP_FILTER: number
    const ICAP_FLASHUSED: number
    const ICAP_GAMMA: number
    const ICAP_HALFTONES: number
    const ICAP_HIGHLIGHT: number
    const ICAP_IMAGEFILEFORMAT: number
    const ICAP_LAMPSTATE: number
    const ICAP_LIGHTSOURCE: number
    const ICAP_ORIENTATION: number
    const ICAP_PHYSICALWIDTH: number
    const ICAP_PHYSICALHEIGHT: number
    const ICAP_SHADOW: number
    const ICAP_FRAMES: number
    const ICAP_XNATIVERESOLUTION: number
    const ICAP_YNATIVERESOLUTION: number
    const ICAP_XRESOLUTION: number
    const ICAP_YRESOLUTION: number
    const ICAP_MAXFRAMES: number
    const ICAP_TILES: number
    const ICAP_BITORDER: number
    const ICAP_CCITTKFACTOR: number
    const ICAP_LIGHTPATH: number
    const ICAP_PIXELFLAVOR: number
    const ICAP_PLANARCHUNKY: number
    const ICAP_ROTATION: number
    const ICAP_SUPPORTEDSIZES: number
    const ICAP_THRESHOLD: number
    const ICAP_XSCALING: number
    const ICAP_YSCALING: number
    const ICAP_BITORDERCODES: number
    const ICAP_PIXELFLAVORCODES: number
    const ICAP_JPEGPIXELTYPE: number
    const ICAP_TIMEFILL: number
    const ICAP_BITDEPTH: number
    const ICAP_BITDEPTHREDUCTION: number
    const ICAP_UNDEFINEDIMAGESIZE: number
    const ICAP_IMAGEDATASET: number
    const ICAP_EXTIMAGEINFO: number
    const ICAP_MINIMUMHEIGHT: number
    const ICAP_MINIMUMWIDTH: number
    const ICAP_AUTODISCARDBLANKPAGES: number
    const ICAP_FLIPROTATION: number
    const ICAP_BARCODEDETECTIONENABLED: number
    const ICAP_SUPPORTEDBARCODETYPES: number
    const ICAP_BARCODEMAXSEARCHPRIORITIES: number
    const ICAP_BARCODESEARCHPRIORITIES: number
    const ICAP_BARCODESEARCHMODE: number
    const ICAP_BARCODEMAXRETRIES: number
    const ICAP_BARCODETIMEOUT: number
    const ICAP_ZOOMFACTOR: number
    const ICAP_PATCHCODEDETECTIONENABLED: number
    const ICAP_SUPPORTEDPATCHCODETYPES: number
    const ICAP_PATCHCODEMAXSEARCHPRIORITIES: number
    const ICAP_PATCHCODESEARCHPRIORITIES: number
    const ICAP_PATCHCODESEARCHMODE: number
    const ICAP_PATCHCODEMAXRETRIES: number
    const ICAP_PATCHCODETIMEOUT: number
    const ICAP_FLASHUSED2: number
    const ICAP_IMAGEFILTER: number
    const ICAP_NOISEFILTER: number
    const ICAP_OVERSCAN: number
    const ICAP_AUTOMATICBORDERDETECTION: number
    const ICAP_AUTOMATICDESKEW: number
    const ICAP_AUTOMATICROTATE: number
    const ICAP_JPEGQUALITY: number
    const ICAP_FEEDERTYPE: number
    const ICAP_ICCPROFILE: number
    const ICAP_AUTOSIZE: number
    const ICAP_AUTOMATICCROPUSESFRAME: number
    const ICAP_AUTOMATICLENGTHDETECTION: number
    const ICAP_AUTOMATICCOLORENABLED: number
    const ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE: number
    const ICAP_COLORMANAGEMENTENABLED: number
    const ICAP_IMAGEMERGE: number
    const ICAP_IMAGEMERGEHEIGHTTHRESHOLD: number
    const ICAP_SUPPORTEDEXTIMAGEINFO: number
    const ICAP_FILMTYPE: number
    const ICAP_MIRROR: number
    const ICAP_JPEGSUBSAMPLING: number
    const ACAP_XFERMECH: number

    const TWSX_NATIVE: number
    const TWSX_FILE: number
    const TWSX_MEMORY: number
    const TWSX_MEMFILE: number

    declare class TwainSDK {
        state: number = 1

        constructor(identify: Identity)

        getDataSources: () => string[];
        getDefaultSource: () => string;
        setDefaultSource: (name: string) => void;
        openDataSource: (sourceName?: string) => Promise<void>;
        getCapability: (capability: number) => Capability;
        setCallback: () => void;
        enableDataSource: () => Promise<void>;
        scan: (transfer: Number, fileName: string) => void;
    }
}