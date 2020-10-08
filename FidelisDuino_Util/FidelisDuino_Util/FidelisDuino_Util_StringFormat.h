#pragma once

namespace FidelisDuino {
	namespace Util {
		namespace Text {
			namespace Format {
				//SplitStringByDelimiter: 
				//https://stackoverflow.com/questions/29671455/how-to-split-a-string-using-a-specific-delimiter-in-arduino
				String SplitStringByDelimiter(String data, char separator, int index)
				{
					int found = 0;
					int strIndex[] = { 0, -1 };
					int maxIndex = data.length() - 1;

					for (int i = 0; i <= maxIndex && found <= index; i++) {
						#if defined( ___DEBUG_ON___ )
							if (data.at(i) == separator || i == maxIndex) {
								found++;
								strIndex[0] = strIndex[1] + 1;
								strIndex[1] = (i == maxIndex) ? i + 1 : i;
							}
						#else
							if (data.charAt(i) == separator || i == maxIndex) {
								found++;
								strIndex[0] = strIndex[1] + 1;
								strIndex[1] = (i == maxIndex) ? i + 1 : i;
							}
						#endif
						
					}
					#if defined( ___DEBUG_ON___ )
						return found > index ? data.substr(strIndex[0], strIndex[1]) : "";
					#else
						return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
					#endif
				}

				// Function to convert hexadecimal to decimal:
				//https://www.geeksforgeeks.org/program-for-hexadecimal-to-decimal/
				int hexadecimalToDecimal(String hexVal)
				{
					int len = hexVal.length();

					// Initializing base value to 1, i.e 16^0 
					int base = 1;

					int dec_val = 0;

					// Extracting characters as digits from last character 
					for (int i = len - 1; i >= 0; i--)
					{
						// if character lies in '0'-'9', converting  
						// it to integral 0-9 by subtracting 48 from 
						// ASCII value. 
						if (hexVal[i] >= '0' && hexVal[i] <= '9')
						{
							dec_val += (hexVal[i] - 48) * base;

							// incrementing base by power 
							base = base * 16;
						}

						// if character lies in 'A'-'F' , converting  
						// it to integral 10 - 15 by subtracting 55  
						// from ASCII value 
						else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
						{
							dec_val += (hexVal[i] - 55) * base;

							// incrementing base by power 
							base = base * 16;
						}
					}

					return dec_val;
				}

				//Formatação MAC Address:
				//https://arduinobasics.blogspot.com/2019/05/sprintf-function.html
				String MACFormat(uint8_t* mac) {
					//uint8_t mac[6];
					char macStr[18] = { 0 };
					//sprintf -> compilador deu um alerta com essa função. TODO: Teste se com sprintf tbem funciona!
					sprintf_s(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
					//sprintf(macStr, "%d:%d:%d:%d:%d:%d", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
					return String(macStr);
				}

				uint8_t* MACFormat(String mac) {
					static uint8_t result[6];
					//static uint8_t m1,m2,m3,m4,m5,m6;

					String mac01 = SplitStringByDelimiter(mac, ':', 0);
					String mac02 = SplitStringByDelimiter(mac, ':', 1);
					String mac03 = SplitStringByDelimiter(mac, ':', 2);
					String mac04 = SplitStringByDelimiter(mac, ':', 3);
					String mac05 = SplitStringByDelimiter(mac, ':', 4);
					String mac06 = SplitStringByDelimiter(mac, ':', 5);

					result[0] = hexadecimalToDecimal(mac01);
					result[1] = hexadecimalToDecimal(mac02);
					result[2] = hexadecimalToDecimal(mac03);
					result[3] = hexadecimalToDecimal(mac04);
					result[4] = hexadecimalToDecimal(mac05);
					result[5] = hexadecimalToDecimal(mac06);

					//Serial.println(m1,m2);

					return result;
				}

				void Tester_MACFormat() {
					//Array com MAC Address em qualquer formato:
					uint8_t mac_int[6] = { 0xD,3,4,5,6,0x7A };

					//Converte array em string formatada:
					String mac_str = MACFormat(mac_int); //Resultado: mac_str = "0D:03:04:05:06:7A"

					//Converte string formatada em array com MAC Address:
					uint8_t* mac_array = MACFormat(mac_str); //Resultado: mac_array = {13,3,4,5,6,122}

					#if defined( ___DEBUG_ON___ )
					//Nothing to do here!
					#else
					Serial.println(mac_str);Serial.println(*mac_array);
					#endif
				}
			}
		}
	}
}