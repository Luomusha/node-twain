#include <assert.h>
#include "TwainSDK.h"

Napi::Function registerTwainSDK(Napi::Env env) {
    // This method is used to hook the accessor and method callbacks
    Napi::Function func = TwainSDK::DefineClass(env, "TwainSDK", {
            TwainSDK::InstanceMethod<&TwainSDK::getState>(
                    "getState",
                    static_cast<napi_property_attributes>(napi_writable | napi_configurable)
            ),
            TwainSDK::InstanceMethod<&TwainSDK::getDataSources>(
                    "getDataSources",
                    static_cast<napi_property_attributes>(napi_writable | napi_configurable)
            ),
            TwainSDK::InstanceMethod<&TwainSDK::getDefaultSource>(
                    "getDefaultSource",
                    static_cast<napi_property_attributes>(napi_writable | napi_configurable)
            ),
            TwainSDK::InstanceMethod<&TwainSDK::setDefaultSource>(
                    "setDefaultSource",
                    static_cast<napi_property_attributes>(napi_writable | napi_configurable)
            ),
            TwainSDK::InstanceMethod<&TwainSDK::openDataSource>(
                    "openDataSource",
                    static_cast<napi_property_attributes>(napi_writable | napi_configurable)
            ),
            TwainSDK::InstanceMethod<&TwainSDK::getCapability>(
                    "getCapability",
                    static_cast<napi_property_attributes>(napi_writable | napi_configurable)
            ),
            TwainSDK::InstanceMethod<&TwainSDK::setCallback>(
                    "setCallback",
                    static_cast<napi_property_attributes>(napi_writable | napi_configurable)
            ),
            TwainSDK::InstanceMethod<&TwainSDK::enableDataSource>(
                    "enableDataSource",
                    static_cast<napi_property_attributes>(napi_writable | napi_configurable)
            ),
            TwainSDK::InstanceMethod<&TwainSDK::scan>(
                    "scan",
                    static_cast<napi_property_attributes>(napi_writable | napi_configurable)
            ),
    });

    // Create a persistent reference to the class constructor. This will allow
    // a function called on a class prototype and a function
    // called on instance of a class to be distinguished from each other.
    Napi::FunctionReference *constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    return func;
}

Napi::Object init(Napi::Env env, Napi::Object exports) {
    exports.Set("TwainSDK", registerTwainSDK(env));

    /****************************************************************************
     * Country Constants                                                        *
     ****************************************************************************/
    exports.Set("TWCY_AFGHANISTAN", TWCY_AFGHANISTAN);
    exports.Set("TWCY_ALGERIA", TWCY_ALGERIA);
    exports.Set("TWCY_AMERICANSAMOA", TWCY_AMERICANSAMOA);
    exports.Set("TWCY_ANDORRA", TWCY_ANDORRA);
    exports.Set("TWCY_ANGOLA", TWCY_ANGOLA);
    exports.Set("TWCY_ANGUILLA", TWCY_ANGUILLA);
    exports.Set("TWCY_ANTIGUA", TWCY_ANTIGUA);
    exports.Set("TWCY_ARGENTINA", TWCY_ARGENTINA);
    exports.Set("TWCY_ARUBA", TWCY_ARUBA);
    exports.Set("TWCY_ASCENSIONI", TWCY_ASCENSIONI);
    exports.Set("TWCY_AUSTRALIA", TWCY_AUSTRALIA);
    exports.Set("TWCY_AUSTRIA", TWCY_AUSTRIA);
    exports.Set("TWCY_BAHAMAS", TWCY_BAHAMAS);
    exports.Set("TWCY_BAHRAIN", TWCY_BAHRAIN);
    exports.Set("TWCY_BANGLADESH", TWCY_BANGLADESH);
    exports.Set("TWCY_BARBADOS", TWCY_BARBADOS);
    exports.Set("TWCY_BELGIUM", TWCY_BELGIUM);
    exports.Set("TWCY_BELIZE", TWCY_BELIZE);
    exports.Set("TWCY_BENIN", TWCY_BENIN);
    exports.Set("TWCY_BERMUDA", TWCY_BERMUDA);
    exports.Set("TWCY_BHUTAN", TWCY_BHUTAN);
    exports.Set("TWCY_BOLIVIA", TWCY_BOLIVIA);
    exports.Set("TWCY_BOTSWANA", TWCY_BOTSWANA);
    exports.Set("TWCY_BRITAIN", TWCY_BRITAIN);
    exports.Set("TWCY_BRITVIRGINIS", TWCY_BRITVIRGINIS);
    exports.Set("TWCY_BRAZIL", TWCY_BRAZIL);
    exports.Set("TWCY_BRUNEI", TWCY_BRUNEI);
    exports.Set("TWCY_BULGARIA", TWCY_BULGARIA);
    exports.Set("TWCY_BURKINAFASO", TWCY_BURKINAFASO);
    exports.Set("TWCY_BURMA", TWCY_BURMA);
    exports.Set("TWCY_BURUNDI", TWCY_BURUNDI);
    exports.Set("TWCY_CAMAROON", TWCY_CAMAROON);
    exports.Set("TWCY_CANADA", TWCY_CANADA);
    exports.Set("TWCY_CAPEVERDEIS", TWCY_CAPEVERDEIS);
    exports.Set("TWCY_CAYMANIS", TWCY_CAYMANIS);
    exports.Set("TWCY_CENTRALAFREP", TWCY_CENTRALAFREP);
    exports.Set("TWCY_CHAD", TWCY_CHAD);
    exports.Set("TWCY_CHILE", TWCY_CHILE);
    exports.Set("TWCY_CHINA", TWCY_CHINA);
    exports.Set("TWCY_CHRISTMASIS", TWCY_CHRISTMASIS);
    exports.Set("TWCY_COCOSIS", TWCY_COCOSIS);
    exports.Set("TWCY_COLOMBIA", TWCY_COLOMBIA);
    exports.Set("TWCY_COMOROS", TWCY_COMOROS);
    exports.Set("TWCY_CONGO", TWCY_CONGO);
    exports.Set("TWCY_COOKIS", TWCY_COOKIS);
    exports.Set("TWCY_COSTARICA", TWCY_COSTARICA);
    exports.Set("TWCY_CUBA", TWCY_CUBA);
    exports.Set("TWCY_CYPRUS", TWCY_CYPRUS);
    exports.Set("TWCY_CZECHOSLOVAKIA", TWCY_CZECHOSLOVAKIA);
    exports.Set("TWCY_DENMARK", TWCY_DENMARK);
    exports.Set("TWCY_DJIBOUTI", TWCY_DJIBOUTI);
    exports.Set("TWCY_DOMINICA", TWCY_DOMINICA);
    exports.Set("TWCY_DOMINCANREP", TWCY_DOMINCANREP);
    exports.Set("TWCY_EASTERIS", TWCY_EASTERIS);
    exports.Set("TWCY_ECUADOR", TWCY_ECUADOR);
    exports.Set("TWCY_EGYPT", TWCY_EGYPT);
    exports.Set("TWCY_ELSALVADOR", TWCY_ELSALVADOR);
    exports.Set("TWCY_EQGUINEA", TWCY_EQGUINEA);
    exports.Set("TWCY_ETHIOPIA", TWCY_ETHIOPIA);
    exports.Set("TWCY_FALKLANDIS", TWCY_FALKLANDIS);
    exports.Set("TWCY_FAEROEIS", TWCY_FAEROEIS);
    exports.Set("TWCY_FIJIISLANDS", TWCY_FIJIISLANDS);
    exports.Set("TWCY_FINLAND", TWCY_FINLAND);
    exports.Set("TWCY_FRANCE", TWCY_FRANCE);
    exports.Set("TWCY_FRANTILLES", TWCY_FRANTILLES);
    exports.Set("TWCY_FRGUIANA", TWCY_FRGUIANA);
    exports.Set("TWCY_FRPOLYNEISA", TWCY_FRPOLYNEISA);
    exports.Set("TWCY_FUTANAIS", TWCY_FUTANAIS);
    exports.Set("TWCY_GABON", TWCY_GABON);
    exports.Set("TWCY_GAMBIA", TWCY_GAMBIA);
    exports.Set("TWCY_GERMANY", TWCY_GERMANY);
    exports.Set("TWCY_GHANA", TWCY_GHANA);
    exports.Set("TWCY_GIBRALTER", TWCY_GIBRALTER);
    exports.Set("TWCY_GREECE", TWCY_GREECE);
    exports.Set("TWCY_GREENLAND", TWCY_GREENLAND);
    exports.Set("TWCY_GRENADA", TWCY_GRENADA);
    exports.Set("TWCY_GRENEDINES", TWCY_GRENEDINES);
    exports.Set("TWCY_GUADELOUPE", TWCY_GUADELOUPE);
    exports.Set("TWCY_GUAM", TWCY_GUAM);
    exports.Set("TWCY_GUANTANAMOBAY", TWCY_GUANTANAMOBAY);
    exports.Set("TWCY_GUATEMALA", TWCY_GUATEMALA);
    exports.Set("TWCY_GUINEA", TWCY_GUINEA);
    exports.Set("TWCY_GUINEABISSAU", TWCY_GUINEABISSAU);
    exports.Set("TWCY_GUYANA", TWCY_GUYANA);
    exports.Set("TWCY_HAITI", TWCY_HAITI);
    exports.Set("TWCY_HONDURAS", TWCY_HONDURAS);
    exports.Set("TWCY_HONGKONG", TWCY_HONGKONG);
    exports.Set("TWCY_HUNGARY", TWCY_HUNGARY);
    exports.Set("TWCY_ICELAND", TWCY_ICELAND);
    exports.Set("TWCY_INDIA", TWCY_INDIA);
    exports.Set("TWCY_INDONESIA", TWCY_INDONESIA);
    exports.Set("TWCY_IRAN", TWCY_IRAN);
    exports.Set("TWCY_IRAQ", TWCY_IRAQ);
    exports.Set("TWCY_IRELAND", TWCY_IRELAND);
    exports.Set("TWCY_ISRAEL", TWCY_ISRAEL);
    exports.Set("TWCY_ITALY", TWCY_ITALY);
    exports.Set("TWCY_IVORYCOAST", TWCY_IVORYCOAST);
    exports.Set("TWCY_JAMAICA", TWCY_JAMAICA);
    exports.Set("TWCY_JAPAN", TWCY_JAPAN);
    exports.Set("TWCY_JORDAN", TWCY_JORDAN);
    exports.Set("TWCY_KENYA", TWCY_KENYA);
    exports.Set("TWCY_KIRIBATI", TWCY_KIRIBATI);
    exports.Set("TWCY_KOREA", TWCY_KOREA);
    exports.Set("TWCY_KUWAIT", TWCY_KUWAIT);
    exports.Set("TWCY_LAOS", TWCY_LAOS);
    exports.Set("TWCY_LEBANON", TWCY_LEBANON);
    exports.Set("TWCY_LIBERIA", TWCY_LIBERIA);
    exports.Set("TWCY_LIBYA", TWCY_LIBYA);
    exports.Set("TWCY_LIECHTENSTEIN", TWCY_LIECHTENSTEIN);
    exports.Set("TWCY_LUXENBOURG", TWCY_LUXENBOURG);
    exports.Set("TWCY_MACAO", TWCY_MACAO);
    exports.Set("TWCY_MADAGASCAR", TWCY_MADAGASCAR);
    exports.Set("TWCY_MALAWI", TWCY_MALAWI);
    exports.Set("TWCY_MALAYSIA", TWCY_MALAYSIA);
    exports.Set("TWCY_MALDIVES", TWCY_MALDIVES);
    exports.Set("TWCY_MALI", TWCY_MALI);
    exports.Set("TWCY_MALTA", TWCY_MALTA);
    exports.Set("TWCY_MARSHALLIS", TWCY_MARSHALLIS);
    exports.Set("TWCY_MAURITANIA", TWCY_MAURITANIA);
    exports.Set("TWCY_MAURITIUS", TWCY_MAURITIUS);
    exports.Set("TWCY_MEXICO", TWCY_MEXICO);
    exports.Set("TWCY_MICRONESIA", TWCY_MICRONESIA);
    exports.Set("TWCY_MIQUELON", TWCY_MIQUELON);
    exports.Set("TWCY_MONACO", TWCY_MONACO);
    exports.Set("TWCY_MONGOLIA", TWCY_MONGOLIA);
    exports.Set("TWCY_MONTSERRAT", TWCY_MONTSERRAT);
    exports.Set("TWCY_MOROCCO", TWCY_MOROCCO);
    exports.Set("TWCY_MOZAMBIQUE", TWCY_MOZAMBIQUE);
    exports.Set("TWCY_NAMIBIA", TWCY_NAMIBIA);
    exports.Set("TWCY_NAURU", TWCY_NAURU);
    exports.Set("TWCY_NEPAL", TWCY_NEPAL);
    exports.Set("TWCY_NETHERLANDS", TWCY_NETHERLANDS);
    exports.Set("TWCY_NETHANTILLES", TWCY_NETHANTILLES);
    exports.Set("TWCY_NEVIS", TWCY_NEVIS);
    exports.Set("TWCY_NEWCALEDONIA", TWCY_NEWCALEDONIA);
    exports.Set("TWCY_NEWZEALAND", TWCY_NEWZEALAND);
    exports.Set("TWCY_NICARAGUA", TWCY_NICARAGUA);
    exports.Set("TWCY_NIGER", TWCY_NIGER);
    exports.Set("TWCY_NIGERIA", TWCY_NIGERIA);
    exports.Set("TWCY_NIUE", TWCY_NIUE);
    exports.Set("TWCY_NORFOLKI", TWCY_NORFOLKI);
    exports.Set("TWCY_NORWAY", TWCY_NORWAY);
    exports.Set("TWCY_OMAN", TWCY_OMAN);
    exports.Set("TWCY_PAKISTAN", TWCY_PAKISTAN);
    exports.Set("TWCY_PALAU", TWCY_PALAU);
    exports.Set("TWCY_PANAMA", TWCY_PANAMA);
    exports.Set("TWCY_PARAGUAY", TWCY_PARAGUAY);
    exports.Set("TWCY_PERU", TWCY_PERU);
    exports.Set("TWCY_PHILLIPPINES", TWCY_PHILLIPPINES);
    exports.Set("TWCY_PITCAIRNIS", TWCY_PITCAIRNIS);
    exports.Set("TWCY_PNEWGUINEA", TWCY_PNEWGUINEA);
    exports.Set("TWCY_POLAND", TWCY_POLAND);
    exports.Set("TWCY_PORTUGAL", TWCY_PORTUGAL);
    exports.Set("TWCY_QATAR", TWCY_QATAR);
    exports.Set("TWCY_REUNIONI", TWCY_REUNIONI);
    exports.Set("TWCY_ROMANIA", TWCY_ROMANIA);
    exports.Set("TWCY_RWANDA", TWCY_RWANDA);
    exports.Set("TWCY_SAIPAN", TWCY_SAIPAN);
    exports.Set("TWCY_SANMARINO", TWCY_SANMARINO);
    exports.Set("TWCY_SAOTOME", TWCY_SAOTOME);
    exports.Set("TWCY_SAUDIARABIA", TWCY_SAUDIARABIA);
    exports.Set("TWCY_SENEGAL", TWCY_SENEGAL);
    exports.Set("TWCY_SEYCHELLESIS", TWCY_SEYCHELLESIS);
    exports.Set("TWCY_SIERRALEONE", TWCY_SIERRALEONE);
    exports.Set("TWCY_SINGAPORE", TWCY_SINGAPORE);
    exports.Set("TWCY_SOLOMONIS", TWCY_SOLOMONIS);
    exports.Set("TWCY_SOMALI", TWCY_SOMALI);
    exports.Set("TWCY_SOUTHAFRICA", TWCY_SOUTHAFRICA);
    exports.Set("TWCY_SPAIN", TWCY_SPAIN);
    exports.Set("TWCY_SRILANKA", TWCY_SRILANKA);
    exports.Set("TWCY_STHELENA", TWCY_STHELENA);
    exports.Set("TWCY_STKITTS", TWCY_STKITTS);
    exports.Set("TWCY_STLUCIA", TWCY_STLUCIA);
    exports.Set("TWCY_STPIERRE", TWCY_STPIERRE);
    exports.Set("TWCY_STVINCENT", TWCY_STVINCENT);
    exports.Set("TWCY_SUDAN", TWCY_SUDAN);
    exports.Set("TWCY_SURINAME", TWCY_SURINAME);
    exports.Set("TWCY_SWAZILAND", TWCY_SWAZILAND);
    exports.Set("TWCY_SWEDEN", TWCY_SWEDEN);
    exports.Set("TWCY_SWITZERLAND", TWCY_SWITZERLAND);
    exports.Set("TWCY_SYRIA", TWCY_SYRIA);
    exports.Set("TWCY_TAIWAN", TWCY_TAIWAN);
    exports.Set("TWCY_TANZANIA", TWCY_TANZANIA);
    exports.Set("TWCY_THAILAND", TWCY_THAILAND);
    exports.Set("TWCY_TOBAGO", TWCY_TOBAGO);
    exports.Set("TWCY_TOGO", TWCY_TOGO);
    exports.Set("TWCY_TONGAIS", TWCY_TONGAIS);
    exports.Set("TWCY_TRINIDAD", TWCY_TRINIDAD);
    exports.Set("TWCY_TUNISIA", TWCY_TUNISIA);
    exports.Set("TWCY_TURKEY", TWCY_TURKEY);
    exports.Set("TWCY_TURKSCAICOS", TWCY_TURKSCAICOS);
    exports.Set("TWCY_TUVALU", TWCY_TUVALU);
    exports.Set("TWCY_UGANDA", TWCY_UGANDA);
    exports.Set("TWCY_USSR", TWCY_USSR);
    exports.Set("TWCY_UAEMIRATES", TWCY_UAEMIRATES);
    exports.Set("TWCY_UNITEDKINGDOM", TWCY_UNITEDKINGDOM);
    exports.Set("TWCY_USA", TWCY_USA);
    exports.Set("TWCY_URUGUAY", TWCY_URUGUAY);
    exports.Set("TWCY_VANUATU", TWCY_VANUATU);
    exports.Set("TWCY_VATICANCITY", TWCY_VATICANCITY);
    exports.Set("TWCY_VENEZUELA", TWCY_VENEZUELA);
    exports.Set("TWCY_WAKE", TWCY_WAKE);
    exports.Set("TWCY_WALLISIS", TWCY_WALLISIS);
    exports.Set("TWCY_WESTERNSAHARA", TWCY_WESTERNSAHARA);
    exports.Set("TWCY_WESTERNSAMOA", TWCY_WESTERNSAMOA);
    exports.Set("TWCY_YEMEN", TWCY_YEMEN);
    exports.Set("TWCY_YUGOSLAVIA", TWCY_YUGOSLAVIA);
    exports.Set("TWCY_ZAIRE", TWCY_ZAIRE);
    exports.Set("TWCY_ZAMBIA", TWCY_ZAMBIA);
    exports.Set("TWCY_ZIMBABWE", TWCY_ZIMBABWE);
    exports.Set("TWCY_ALBANIA", TWCY_ALBANIA);
    exports.Set("TWCY_ARMENIA", TWCY_ARMENIA);
    exports.Set("TWCY_AZERBAIJAN", TWCY_AZERBAIJAN);
    exports.Set("TWCY_BELARUS", TWCY_BELARUS);
    exports.Set("TWCY_BOSNIAHERZGO", TWCY_BOSNIAHERZGO);
    exports.Set("TWCY_CAMBODIA", TWCY_CAMBODIA);
    exports.Set("TWCY_CROATIA", TWCY_CROATIA);
    exports.Set("TWCY_CZECHREPUBLIC", TWCY_CZECHREPUBLIC);
    exports.Set("TWCY_DIEGOGARCIA", TWCY_DIEGOGARCIA);
    exports.Set("TWCY_ERITREA", TWCY_ERITREA);
    exports.Set("TWCY_ESTONIA", TWCY_ESTONIA);
    exports.Set("TWCY_GEORGIA", TWCY_GEORGIA);
    exports.Set("TWCY_LATVIA", TWCY_LATVIA);
    exports.Set("TWCY_LESOTHO", TWCY_LESOTHO);
    exports.Set("TWCY_LITHUANIA", TWCY_LITHUANIA);
    exports.Set("TWCY_MACEDONIA", TWCY_MACEDONIA);
    exports.Set("TWCY_MAYOTTEIS", TWCY_MAYOTTEIS);
    exports.Set("TWCY_MOLDOVA", TWCY_MOLDOVA);
    exports.Set("TWCY_MYANMAR", TWCY_MYANMAR);
    exports.Set("TWCY_NORTHKOREA", TWCY_NORTHKOREA);
    exports.Set("TWCY_PUERTORICO", TWCY_PUERTORICO);
    exports.Set("TWCY_RUSSIA", TWCY_RUSSIA);
    exports.Set("TWCY_SERBIA", TWCY_SERBIA);
    exports.Set("TWCY_SLOVAKIA", TWCY_SLOVAKIA);
    exports.Set("TWCY_SLOVENIA", TWCY_SLOVENIA);
    exports.Set("TWCY_SOUTHKOREA", TWCY_SOUTHKOREA);
    exports.Set("TWCY_UKRAINE", TWCY_UKRAINE);
    exports.Set("TWCY_USVIRGINIS", TWCY_USVIRGINIS);
    exports.Set("TWCY_VIETNAM", TWCY_VIETNAM);

    /****************************************************************************
     * Language Constants                                                       *
     ****************************************************************************/
    exports.Set("TWLG_USERLOCALE", TWLG_USERLOCALE);
    exports.Set("TWLG_DAN", TWLG_DAN);
    exports.Set("TWLG_DUT", TWLG_DUT);
    exports.Set("TWLG_ENG", TWLG_ENG);
    exports.Set("TWLG_FCF", TWLG_FCF);
    exports.Set("TWLG_FIN", TWLG_FIN);
    exports.Set("TWLG_FRN", TWLG_FRN);
    exports.Set("TWLG_GER", TWLG_GER);
    exports.Set("TWLG_ICE", TWLG_ICE);
    exports.Set("TWLG_ITN", TWLG_ITN);
    exports.Set("TWLG_NOR", TWLG_NOR);
    exports.Set("TWLG_POR", TWLG_POR);
    exports.Set("TWLG_SPA", TWLG_SPA);
    exports.Set("TWLG_SWE", TWLG_SWE);
    exports.Set("TWLG_USA", TWLG_USA);
    exports.Set("TWLG_AFRIKAANS", TWLG_AFRIKAANS);
    exports.Set("TWLG_ALBANIA", TWLG_ALBANIA);
    exports.Set("TWLG_ARABIC", TWLG_ARABIC);
    exports.Set("TWLG_ARABIC_ALGERIA", TWLG_ARABIC_ALGERIA);
    exports.Set("TWLG_ARABIC_BAHRAIN", TWLG_ARABIC_BAHRAIN);
    exports.Set("TWLG_ARABIC_EGYPT", TWLG_ARABIC_EGYPT);
    exports.Set("TWLG_ARABIC_IRAQ", TWLG_ARABIC_IRAQ);
    exports.Set("TWLG_ARABIC_JORDAN", TWLG_ARABIC_JORDAN);
    exports.Set("TWLG_ARABIC_KUWAIT", TWLG_ARABIC_KUWAIT);
    exports.Set("TWLG_ARABIC_LEBANON", TWLG_ARABIC_LEBANON);
    exports.Set("TWLG_ARABIC_LIBYA", TWLG_ARABIC_LIBYA);
    exports.Set("TWLG_ARABIC_MOROCCO", TWLG_ARABIC_MOROCCO);
    exports.Set("TWLG_ARABIC_OMAN", TWLG_ARABIC_OMAN);
    exports.Set("TWLG_ARABIC_QATAR", TWLG_ARABIC_QATAR);
    exports.Set("TWLG_ARABIC_SAUDIARABIA", TWLG_ARABIC_SAUDIARABIA);
    exports.Set("TWLG_ARABIC_SYRIA", TWLG_ARABIC_SYRIA);
    exports.Set("TWLG_ARABIC_TUNISIA", TWLG_ARABIC_TUNISIA);
    exports.Set("TWLG_ARABIC_UAE", TWLG_ARABIC_UAE);
    exports.Set("TWLG_ARABIC_YEMEN", TWLG_ARABIC_YEMEN);
    exports.Set("TWLG_BASQUE", TWLG_BASQUE);
    exports.Set("TWLG_BYELORUSSIAN", TWLG_BYELORUSSIAN);
    exports.Set("TWLG_BULGARIAN", TWLG_BULGARIAN);
    exports.Set("TWLG_CATALAN", TWLG_CATALAN);
    exports.Set("TWLG_CHINESE", TWLG_CHINESE);
    exports.Set("TWLG_CHINESE_HONGKONG", TWLG_CHINESE_HONGKONG);
    exports.Set("TWLG_CHINESE_PRC", TWLG_CHINESE_PRC);
    exports.Set("TWLG_CHINESE_SINGAPORE", TWLG_CHINESE_SINGAPORE);
    exports.Set("TWLG_CHINESE_SIMPLIFIED", TWLG_CHINESE_SIMPLIFIED);
    exports.Set("TWLG_CHINESE_TAIWAN", TWLG_CHINESE_TAIWAN);
    exports.Set("TWLG_CHINESE_TRADITIONAL", TWLG_CHINESE_TRADITIONAL);
    exports.Set("TWLG_CROATIA", TWLG_CROATIA);
    exports.Set("TWLG_CZECH", TWLG_CZECH);
    exports.Set("TWLG_DANISH", TWLG_DANISH);
    exports.Set("TWLG_DUTCH", TWLG_DUTCH);
    exports.Set("TWLG_DUTCH_BELGIAN", TWLG_DUTCH_BELGIAN);
    exports.Set("TWLG_ENGLISH", TWLG_ENGLISH);
    exports.Set("TWLG_ENGLISH_AUSTRALIAN", TWLG_ENGLISH_AUSTRALIAN);
    exports.Set("TWLG_ENGLISH_CANADIAN", TWLG_ENGLISH_CANADIAN);
    exports.Set("TWLG_ENGLISH_IRELAND", TWLG_ENGLISH_IRELAND);
    exports.Set("TWLG_ENGLISH_NEWZEALAND", TWLG_ENGLISH_NEWZEALAND);
    exports.Set("TWLG_ENGLISH_SOUTHAFRICA", TWLG_ENGLISH_SOUTHAFRICA);
    exports.Set("TWLG_ENGLISH_UK", TWLG_ENGLISH_UK);
    exports.Set("TWLG_ENGLISH_USA", TWLG_ENGLISH_USA);
    exports.Set("TWLG_ESTONIAN", TWLG_ESTONIAN);
    exports.Set("TWLG_FAEROESE", TWLG_FAEROESE);
    exports.Set("TWLG_FARSI", TWLG_FARSI);
    exports.Set("TWLG_FINNISH", TWLG_FINNISH);
    exports.Set("TWLG_FRENCH", TWLG_FRENCH);
    exports.Set("TWLG_FRENCH_BELGIAN", TWLG_FRENCH_BELGIAN);
    exports.Set("TWLG_FRENCH_CANADIAN", TWLG_FRENCH_CANADIAN);
    exports.Set("TWLG_FRENCH_LUXEMBOURG", TWLG_FRENCH_LUXEMBOURG);
    exports.Set("TWLG_FRENCH_SWISS", TWLG_FRENCH_SWISS);
    exports.Set("TWLG_GERMAN", TWLG_GERMAN);
    exports.Set("TWLG_GERMAN_AUSTRIAN", TWLG_GERMAN_AUSTRIAN);
    exports.Set("TWLG_GERMAN_LUXEMBOURG", TWLG_GERMAN_LUXEMBOURG);
    exports.Set("TWLG_GERMAN_LIECHTENSTEIN", TWLG_GERMAN_LIECHTENSTEIN);
    exports.Set("TWLG_GERMAN_SWISS", TWLG_GERMAN_SWISS);
    exports.Set("TWLG_GREEK", TWLG_GREEK);
    exports.Set("TWLG_HEBREW", TWLG_HEBREW);
    exports.Set("TWLG_HUNGARIAN", TWLG_HUNGARIAN);
    exports.Set("TWLG_ICELANDIC", TWLG_ICELANDIC);
    exports.Set("TWLG_INDONESIAN", TWLG_INDONESIAN);
    exports.Set("TWLG_ITALIAN", TWLG_ITALIAN);
    exports.Set("TWLG_ITALIAN_SWISS", TWLG_ITALIAN_SWISS);
    exports.Set("TWLG_JAPANESE", TWLG_JAPANESE);
    exports.Set("TWLG_KOREAN", TWLG_KOREAN);
    exports.Set("TWLG_KOREAN_JOHAB", TWLG_KOREAN_JOHAB);
    exports.Set("TWLG_LATVIAN", TWLG_LATVIAN);
    exports.Set("TWLG_LITHUANIAN", TWLG_LITHUANIAN);
    exports.Set("TWLG_NORWEGIAN", TWLG_NORWEGIAN);
    exports.Set("TWLG_NORWEGIAN_BOKMAL", TWLG_NORWEGIAN_BOKMAL);
    exports.Set("TWLG_NORWEGIAN_NYNORSK", TWLG_NORWEGIAN_NYNORSK);
    exports.Set("TWLG_POLISH", TWLG_POLISH);
    exports.Set("TWLG_PORTUGUESE", TWLG_PORTUGUESE);
    exports.Set("TWLG_PORTUGUESE_BRAZIL", TWLG_PORTUGUESE_BRAZIL);
    exports.Set("TWLG_ROMANIAN", TWLG_ROMANIAN);
    exports.Set("TWLG_RUSSIAN", TWLG_RUSSIAN);
    exports.Set("TWLG_SERBIAN_LATIN", TWLG_SERBIAN_LATIN);
    exports.Set("TWLG_SLOVAK", TWLG_SLOVAK);
    exports.Set("TWLG_SLOVENIAN", TWLG_SLOVENIAN);
    exports.Set("TWLG_SPANISH", TWLG_SPANISH);
    exports.Set("TWLG_SPANISH_MEXICAN", TWLG_SPANISH_MEXICAN);
    exports.Set("TWLG_SPANISH_MODERN", TWLG_SPANISH_MODERN);
    exports.Set("TWLG_SWEDISH", TWLG_SWEDISH);
    exports.Set("TWLG_THAI", TWLG_THAI);
    exports.Set("TWLG_TURKISH", TWLG_TURKISH);
    exports.Set("TWLG_UKRANIAN", TWLG_UKRANIAN);
    exports.Set("TWLG_ASSAMESE", TWLG_ASSAMESE);
    exports.Set("TWLG_BENGALI", TWLG_BENGALI);
    exports.Set("TWLG_BIHARI", TWLG_BIHARI);
    exports.Set("TWLG_BODO", TWLG_BODO);
    exports.Set("TWLG_DOGRI", TWLG_DOGRI);
    exports.Set("TWLG_GUJARATI", TWLG_GUJARATI);
    exports.Set("TWLG_HARYANVI", TWLG_HARYANVI);
    exports.Set("TWLG_HINDI", TWLG_HINDI);
    exports.Set("TWLG_KANNADA", TWLG_KANNADA);
    exports.Set("TWLG_KASHMIRI", TWLG_KASHMIRI);
    exports.Set("TWLG_MALAYALAM", TWLG_MALAYALAM);
    exports.Set("TWLG_MARATHI", TWLG_MARATHI);
    exports.Set("TWLG_MARWARI", TWLG_MARWARI);
    exports.Set("TWLG_MEGHALAYAN", TWLG_MEGHALAYAN);
    exports.Set("TWLG_MIZO", TWLG_MIZO);
    exports.Set("TWLG_NAGA", TWLG_NAGA);
    exports.Set("TWLG_ORISSI", TWLG_ORISSI);
    exports.Set("TWLG_PUNJABI", TWLG_PUNJABI);
    exports.Set("TWLG_PUSHTU", TWLG_PUSHTU);
    exports.Set("TWLG_SERBIAN_CYRILLIC", TWLG_SERBIAN_CYRILLIC);
    exports.Set("TWLG_SIKKIMI", TWLG_SIKKIMI);
    exports.Set("TWLG_SWEDISH_FINLAND", TWLG_SWEDISH_FINLAND);
    exports.Set("TWLG_TAMIL", TWLG_TAMIL);
    exports.Set("TWLG_TELUGU", TWLG_TELUGU);
    exports.Set("TWLG_TRIPURI", TWLG_TRIPURI);
    exports.Set("TWLG_URDU", TWLG_URDU);
    exports.Set("TWLG_VIETNAMESE", TWLG_VIETNAMESE);

    // protoal
    exports.Set("TWON_PROTOCOLMAJOR", TWON_PROTOCOLMAJOR);
    exports.Set("TWON_PROTOCOLMINOR", TWON_PROTOCOLMINOR);

    // capability
    exports.Set("CAP_CUSTOMBASE", CAP_CUSTOMBASE);
    exports.Set("CAP_XFERCOUNT", CAP_XFERCOUNT);
    exports.Set("ICAP_COMPRESSION", ICAP_COMPRESSION);
    exports.Set("ICAP_PIXELTYPE", ICAP_PIXELTYPE);
    exports.Set("ICAP_UNITS", ICAP_UNITS);
    exports.Set("ICAP_XFERMECH", ICAP_XFERMECH);
    exports.Set("CAP_AUTHOR", CAP_AUTHOR);
    exports.Set("CAP_CAPTION", CAP_CAPTION);
    exports.Set("CAP_FEEDERENABLED", CAP_FEEDERENABLED);
    exports.Set("CAP_FEEDERLOADED", CAP_FEEDERLOADED);
    exports.Set("CAP_TIMEDATE", CAP_TIMEDATE);
    exports.Set("CAP_SUPPORTEDCAPS", CAP_SUPPORTEDCAPS);
    exports.Set("CAP_EXTENDEDCAPS", CAP_EXTENDEDCAPS);
    exports.Set("CAP_AUTOFEED", CAP_AUTOFEED);
    exports.Set("CAP_CLEARPAGE", CAP_CLEARPAGE);
    exports.Set("CAP_FEEDPAGE", CAP_FEEDPAGE);
    exports.Set("CAP_REWINDPAGE", CAP_REWINDPAGE);
    exports.Set("CAP_INDICATORS", CAP_INDICATORS);
    exports.Set("CAP_PAPERDETECTABLE", CAP_PAPERDETECTABLE);
    exports.Set("CAP_UICONTROLLABLE", CAP_UICONTROLLABLE);
    exports.Set("CAP_DEVICEONLINE", CAP_DEVICEONLINE);
    exports.Set("CAP_AUTOSCAN", CAP_AUTOSCAN);
    exports.Set("CAP_THUMBNAILSENABLED", CAP_THUMBNAILSENABLED);
    exports.Set("CAP_DUPLEX", CAP_DUPLEX);
    exports.Set("CAP_DUPLEXENABLED", CAP_DUPLEXENABLED);
    exports.Set("CAP_ENABLEDSUIONLY", CAP_ENABLEDSUIONLY);
    exports.Set("CAP_CUSTOMDSDATA", CAP_CUSTOMDSDATA);
    exports.Set("CAP_ENDORSER", CAP_ENDORSER);
    exports.Set("CAP_JOBCONTROL", CAP_JOBCONTROL);
    exports.Set("CAP_ALARMS", CAP_ALARMS);
    exports.Set("CAP_ALARMVOLUME", CAP_ALARMVOLUME);
    exports.Set("CAP_AUTOMATICCAPTURE", CAP_AUTOMATICCAPTURE);
    exports.Set("CAP_TIMEBEFOREFIRSTCAPTURE", CAP_TIMEBEFOREFIRSTCAPTURE);
    exports.Set("CAP_TIMEBETWEENCAPTURES", CAP_TIMEBETWEENCAPTURES);
    exports.Set("CAP_MAXBATCHBUFFERS", CAP_MAXBATCHBUFFERS);
    exports.Set("CAP_DEVICETIMEDATE", CAP_DEVICETIMEDATE);
    exports.Set("CAP_POWERSUPPLY", CAP_POWERSUPPLY);
    exports.Set("CAP_CAMERAPREVIEWUI", CAP_CAMERAPREVIEWUI);
    exports.Set("CAP_DEVICEEVENT", CAP_DEVICEEVENT);
    exports.Set("CAP_SERIALNUMBER", CAP_SERIALNUMBER);
    exports.Set("CAP_PRINTER", CAP_PRINTER);
    exports.Set("CAP_PRINTERENABLED", CAP_PRINTERENABLED);
    exports.Set("CAP_PRINTERINDEX", CAP_PRINTERINDEX);
    exports.Set("CAP_PRINTERMODE", CAP_PRINTERMODE);
    exports.Set("CAP_PRINTERSTRING", CAP_PRINTERSTRING);
    exports.Set("CAP_PRINTERSUFFIX", CAP_PRINTERSUFFIX);
    exports.Set("CAP_LANGUAGE", CAP_LANGUAGE);
    exports.Set("CAP_FEEDERALIGNMENT", CAP_FEEDERALIGNMENT);
    exports.Set("CAP_FEEDERORDER", CAP_FEEDERORDER);
    exports.Set("CAP_REACQUIREALLOWED", CAP_REACQUIREALLOWED);
    exports.Set("CAP_BATTERYMINUTES", CAP_BATTERYMINUTES);
    exports.Set("CAP_BATTERYPERCENTAGE", CAP_BATTERYPERCENTAGE);
    exports.Set("CAP_CAMERASIDE", CAP_CAMERASIDE);
    exports.Set("CAP_SEGMENTED", CAP_SEGMENTED);
    exports.Set("CAP_CAMERAENABLED", CAP_CAMERAENABLED);
    exports.Set("CAP_CAMERAORDER", CAP_CAMERAORDER);
    exports.Set("CAP_MICRENABLED", CAP_MICRENABLED);
    exports.Set("CAP_FEEDERPREP", CAP_FEEDERPREP);
    exports.Set("CAP_FEEDERPOCKET", CAP_FEEDERPOCKET);
    exports.Set("CAP_AUTOMATICSENSEMEDIUM", CAP_AUTOMATICSENSEMEDIUM);
    exports.Set("CAP_CUSTOMINTERFACEGUID", CAP_CUSTOMINTERFACEGUID);
    exports.Set("CAP_SUPPORTEDCAPSSEGMENTUNIQUE", CAP_SUPPORTEDCAPSSEGMENTUNIQUE);
    exports.Set("CAP_SUPPORTEDDATS", CAP_SUPPORTEDDATS);
    exports.Set("CAP_DOUBLEFEEDDETECTION", CAP_DOUBLEFEEDDETECTION);
    exports.Set("CAP_DOUBLEFEEDDETECTIONLENGTH", CAP_DOUBLEFEEDDETECTIONLENGTH);
    exports.Set("CAP_DOUBLEFEEDDETECTIONSENSITIVITY", CAP_DOUBLEFEEDDETECTIONSENSITIVITY);
    exports.Set("CAP_DOUBLEFEEDDETECTIONRESPONSE", CAP_DOUBLEFEEDDETECTIONRESPONSE);
    exports.Set("CAP_PAPERHANDLING", CAP_PAPERHANDLING);
    exports.Set("CAP_INDICATORSMODE", CAP_INDICATORSMODE);
    exports.Set("CAP_PRINTERVERTICALOFFSET", CAP_PRINTERVERTICALOFFSET);
    exports.Set("CAP_POWERSAVETIME", CAP_POWERSAVETIME);
    exports.Set("CAP_PRINTERCHARROTATION", CAP_PRINTERCHARROTATION);
    exports.Set("CAP_PRINTERFONTSTYLE", CAP_PRINTERFONTSTYLE);
    exports.Set("CAP_PRINTERINDEXLEADCHAR", CAP_PRINTERINDEXLEADCHAR);
    exports.Set("CAP_PRINTERINDEXMAXVALUE", CAP_PRINTERINDEXMAXVALUE);
    exports.Set("CAP_PRINTERINDEXNUMDIGITS", CAP_PRINTERINDEXNUMDIGITS);
    exports.Set("CAP_PRINTERINDEXSTEP", CAP_PRINTERINDEXSTEP);
    exports.Set("CAP_PRINTERINDEXTRIGGER", CAP_PRINTERINDEXTRIGGER);
    exports.Set("CAP_PRINTERSTRINGPREVIEW", CAP_PRINTERSTRINGPREVIEW);
    exports.Set("CAP_SHEETCOUNT", CAP_SHEETCOUNT);
    exports.Set("ICAP_AUTOBRIGHT", ICAP_AUTOBRIGHT);
    exports.Set("ICAP_BRIGHTNESS", ICAP_BRIGHTNESS);
    exports.Set("ICAP_CONTRAST", ICAP_CONTRAST);
    exports.Set("ICAP_CUSTHALFTONE", ICAP_CUSTHALFTONE);
    exports.Set("ICAP_EXPOSURETIME", ICAP_EXPOSURETIME);
    exports.Set("ICAP_FILTER", ICAP_FILTER);
    exports.Set("ICAP_FLASHUSED", ICAP_FLASHUSED);
    exports.Set("ICAP_GAMMA", ICAP_GAMMA);
    exports.Set("ICAP_HALFTONES", ICAP_HALFTONES);
    exports.Set("ICAP_HIGHLIGHT", ICAP_HIGHLIGHT);
    exports.Set("ICAP_IMAGEFILEFORMAT", ICAP_IMAGEFILEFORMAT);
    exports.Set("ICAP_LAMPSTATE", ICAP_LAMPSTATE);
    exports.Set("ICAP_LIGHTSOURCE", ICAP_LIGHTSOURCE);
    exports.Set("ICAP_ORIENTATION", ICAP_ORIENTATION);
    exports.Set("ICAP_PHYSICALWIDTH", ICAP_PHYSICALWIDTH);
    exports.Set("ICAP_PHYSICALHEIGHT", ICAP_PHYSICALHEIGHT);
    exports.Set("ICAP_SHADOW", ICAP_SHADOW);
    exports.Set("ICAP_FRAMES", ICAP_FRAMES);
    exports.Set("ICAP_XNATIVERESOLUTION", ICAP_XNATIVERESOLUTION);
    exports.Set("ICAP_YNATIVERESOLUTION", ICAP_YNATIVERESOLUTION);
    exports.Set("ICAP_XRESOLUTION", ICAP_XRESOLUTION);
    exports.Set("ICAP_YRESOLUTION", ICAP_YRESOLUTION);
    exports.Set("ICAP_MAXFRAMES", ICAP_MAXFRAMES);
    exports.Set("ICAP_TILES", ICAP_TILES);
    exports.Set("ICAP_BITORDER", ICAP_BITORDER);
    exports.Set("ICAP_CCITTKFACTOR", ICAP_CCITTKFACTOR);
    exports.Set("ICAP_LIGHTPATH", ICAP_LIGHTPATH);
    exports.Set("ICAP_PIXELFLAVOR", ICAP_PIXELFLAVOR);
    exports.Set("ICAP_PLANARCHUNKY", ICAP_PLANARCHUNKY);
    exports.Set("ICAP_ROTATION", ICAP_ROTATION);
    exports.Set("ICAP_SUPPORTEDSIZES", ICAP_SUPPORTEDSIZES);
    exports.Set("ICAP_THRESHOLD", ICAP_THRESHOLD);
    exports.Set("ICAP_XSCALING", ICAP_XSCALING);
    exports.Set("ICAP_YSCALING", ICAP_YSCALING);
    exports.Set("ICAP_BITORDERCODES", ICAP_BITORDERCODES);
    exports.Set("ICAP_PIXELFLAVORCODES", ICAP_PIXELFLAVORCODES);
    exports.Set("ICAP_JPEGPIXELTYPE", ICAP_JPEGPIXELTYPE);
    exports.Set("ICAP_TIMEFILL", ICAP_TIMEFILL);
    exports.Set("ICAP_BITDEPTH", ICAP_BITDEPTH);
    exports.Set("ICAP_BITDEPTHREDUCTION", ICAP_BITDEPTHREDUCTION);
    exports.Set("ICAP_UNDEFINEDIMAGESIZE", ICAP_UNDEFINEDIMAGESIZE);
    exports.Set("ICAP_IMAGEDATASET", ICAP_IMAGEDATASET);
    exports.Set("ICAP_EXTIMAGEINFO", ICAP_EXTIMAGEINFO);
    exports.Set("ICAP_MINIMUMHEIGHT", ICAP_MINIMUMHEIGHT);
    exports.Set("ICAP_MINIMUMWIDTH", ICAP_MINIMUMWIDTH);
    exports.Set("ICAP_AUTODISCARDBLANKPAGES", ICAP_AUTODISCARDBLANKPAGES);
    exports.Set("ICAP_FLIPROTATION", ICAP_FLIPROTATION);
    exports.Set("ICAP_BARCODEDETECTIONENABLED", ICAP_BARCODEDETECTIONENABLED);
    exports.Set("ICAP_SUPPORTEDBARCODETYPES", ICAP_SUPPORTEDBARCODETYPES);
    exports.Set("ICAP_BARCODEMAXSEARCHPRIORITIES", ICAP_BARCODEMAXSEARCHPRIORITIES);
    exports.Set("ICAP_BARCODESEARCHPRIORITIES", ICAP_BARCODESEARCHPRIORITIES);
    exports.Set("ICAP_BARCODESEARCHMODE", ICAP_BARCODESEARCHMODE);
    exports.Set("ICAP_BARCODEMAXRETRIES", ICAP_BARCODEMAXRETRIES);
    exports.Set("ICAP_BARCODETIMEOUT", ICAP_BARCODETIMEOUT);
    exports.Set("ICAP_ZOOMFACTOR", ICAP_ZOOMFACTOR);
    exports.Set("ICAP_PATCHCODEDETECTIONENABLED", ICAP_PATCHCODEDETECTIONENABLED);
    exports.Set("ICAP_SUPPORTEDPATCHCODETYPES", ICAP_SUPPORTEDPATCHCODETYPES);
    exports.Set("ICAP_PATCHCODEMAXSEARCHPRIORITIES", ICAP_PATCHCODEMAXSEARCHPRIORITIES);
    exports.Set("ICAP_PATCHCODESEARCHPRIORITIES", ICAP_PATCHCODESEARCHPRIORITIES);
    exports.Set("ICAP_PATCHCODESEARCHMODE", ICAP_PATCHCODESEARCHMODE);
    exports.Set("ICAP_PATCHCODEMAXRETRIES", ICAP_PATCHCODEMAXRETRIES);
    exports.Set("ICAP_PATCHCODETIMEOUT", ICAP_PATCHCODETIMEOUT);
    exports.Set("ICAP_FLASHUSED2", ICAP_FLASHUSED2);
    exports.Set("ICAP_IMAGEFILTER", ICAP_IMAGEFILTER);
    exports.Set("ICAP_NOISEFILTER", ICAP_NOISEFILTER);
    exports.Set("ICAP_OVERSCAN", ICAP_OVERSCAN);
    exports.Set("ICAP_AUTOMATICBORDERDETECTION", ICAP_AUTOMATICBORDERDETECTION);
    exports.Set("ICAP_AUTOMATICDESKEW", ICAP_AUTOMATICDESKEW);
    exports.Set("ICAP_AUTOMATICROTATE", ICAP_AUTOMATICROTATE);
    exports.Set("ICAP_JPEGQUALITY", ICAP_JPEGQUALITY);
    exports.Set("ICAP_FEEDERTYPE", ICAP_FEEDERTYPE);
    exports.Set("ICAP_ICCPROFILE", ICAP_ICCPROFILE);
    exports.Set("ICAP_AUTOSIZE", ICAP_AUTOSIZE);
    exports.Set("ICAP_AUTOMATICCROPUSESFRAME", ICAP_AUTOMATICCROPUSESFRAME);
    exports.Set("ICAP_AUTOMATICLENGTHDETECTION", ICAP_AUTOMATICLENGTHDETECTION);
    exports.Set("ICAP_AUTOMATICCOLORENABLED", ICAP_AUTOMATICCOLORENABLED);
    exports.Set("ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE", ICAP_AUTOMATICCOLORNONCOLORPIXELTYPE);
    exports.Set("ICAP_COLORMANAGEMENTENABLED", ICAP_COLORMANAGEMENTENABLED);
    exports.Set("ICAP_IMAGEMERGE", ICAP_IMAGEMERGE);
    exports.Set("ICAP_IMAGEMERGEHEIGHTTHRESHOLD", ICAP_IMAGEMERGEHEIGHTTHRESHOLD);
    exports.Set("ICAP_SUPPORTEDEXTIMAGEINFO", ICAP_SUPPORTEDEXTIMAGEINFO);
    exports.Set("ICAP_FILMTYPE", ICAP_FILMTYPE);
    exports.Set("ICAP_MIRROR", ICAP_MIRROR);
    exports.Set("ICAP_JPEGSUBSAMPLING", ICAP_JPEGSUBSAMPLING);
    exports.Set("ACAP_XFERMECH", ACAP_XFERMECH);

    // Transfer
    exports.Set("TWSX_NATIVE", TWSX_NATIVE);
    exports.Set("TWSX_FILE", TWSX_FILE);
    exports.Set("TWSX_MEMORY", TWSX_MEMORY);
    exports.Set("TWSX_MEMFILE", TWSX_MEMFILE);

    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, init)
