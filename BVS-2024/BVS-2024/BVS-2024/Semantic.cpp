#include "Semantic.h"
#include "Error.h"
#pragma warning(disable : 1041)
namespace SA {

	void operands(MFST::LEX lex) {
		for (int i = 0; i < lex.lextable.size; i++) {

			if (lex.lextable.table[i].lexema == LEX_MORE || lex.lextable.table[i].lexema == LEX_ASSIGNMENT) {

				// Если лексема является оператором присваивания ':'
				if (lex.lextable.table[i].lexema == ':') {
					int cur = -1; 
					IT::IDDATATYPE datatype = (IT::IDDATATYPE)0; 

					while (lex.lextable.table[i + cur].lexema != LEX_SEMICOLON) {

						if (lex.lextable.table[i + cur].lexema == LEX_ID || lex.lextable.table[i + cur].lexema == LEX_LITERAL) {

							if (datatype == (IT::IDDATATYPE)0) {
								datatype = lex.idtable.table[lex.lextable.table[i + cur].idxTI].iddatatype;
							}
							else {

								if (datatype != lex.idtable.table[lex.lextable.table[i + cur].idxTI].iddatatype) {
									throw ERROR_THROW_IN(703, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn);
								}
							}

							if (lex.idtable.table[lex.lextable.table[i + cur].idxTI].idtype == IT::FUNCTION ||
								lex.idtable.table[lex.lextable.table[i + cur].idxTI].idtype == IT::STATIC_FUNCTION) {

								while (lex.lextable.table[i + cur].lexema != LEX_RIGHTTHESIS) {
									cur++;
								}
							}
						}

						// Проверка на допустимость операций с текстовым типом данных (TEXT).
						if (datatype == IT::TEXT && lex.lextable.table[i + cur].lexema == LEX_MORE && cur != 0) {
							throw ERROR_THROW_IN(704, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn);
						}

						// Проверка на допустимость операций с типом данных SYMBOL (символы).
						if (datatype == IT::SYMBOL && lex.lextable.table[i + cur].lexema == LEX_MORE && cur != 0 &&
							lex.lextable.table[i + cur].data[0] != '+' && lex.lextable.table[i + cur].data[0] != '-') {
							throw ERROR_THROW_IN(704, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn);
						}

						// Проверка на допустимость операций с типом данных BOOLEAN (логические значения).
						if (datatype == IT::BOOLEAN && lex.lextable.table[i + cur].lexema == LEX_MORE && cur != 0) {
							throw ERROR_THROW_IN(704, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn);
						}

						cur++; 
					}

					i += cur - 1;
				}
			}

			if (lex.lextable.table[i].lexema == 'b') {
				if (lex.idtable.table[lex.lextable.table[i - 1].idxTI].iddatatype !=
					lex.idtable.table[lex.lextable.table[i + 1].idxTI].iddatatype) {
					throw ERROR_THROW_IN(703, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
				}
			}
		}

		for (int i = 0; i < lex.idtable.size; i++) {

			// Проверяем, что идентификаторы с типом HALLOW используются только как функции
			if (lex.idtable.table[i].iddatatype == IT::HALLOW && lex.idtable.table[i].idtype != IT::FUNCTION) {
				throw ERROR_THROW_IN(708, lex.idtable.table[i].idxfirstLE, lex.idtable.table[i].idxfirstLE);
			}
		}
	}

    void functions(MFST::LEX lex) {
        for (int i = 0; i < lex.lextable.size; i++) {

            if (lex.lextable.table[i].lexema == LEX_DISPLAY) {

                // Если после DISPLAY сразу идет закрывающая скобка, генерируем ошибку
                if (lex.lextable.table[i + 2].lexema == ')') {
                    throw ERROR_THROW_IN(714, lex.lextable.table[i + 2].sn, lex.lextable.table[i + 2].cn);
                }

                IT::IDTYPE re = lex.idtable.table[lex.lextable.table[i + 2].idxTI].idtype;
                IT::IDDATATYPE ree = lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype;

                if (lex.idtable.table[lex.lextable.table[i + 2].idxTI].idtype == IT::FUNCTION &&
                    lex.idtable.table[lex.lextable.table[i + 2].idxTI].iddatatype == IT::HALLOW)
                {
                    throw ERROR_THROW_IN(711, lex.lextable.table[i + 2].sn, lex.lextable.table[i + 2].cn);
                }
            }

            if (lex.lextable.table[i].lexema == LEX_ID &&
                lex.lextable.table[i - 1].lexema == LEX_FUNCTION &&
                lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::FUNCTION)
            {
                int cur = 1; // Переменная для обхода параметров функции
                IT::IDDATATYPE returnType = lex.idtable.table[lex.lextable.table[i].idxTI].iddatatype;

                // Если функция типа HALLOW, то должны быть правило для return
                if (returnType == IT::HALLOW) {
                    while (i + cur < lex.lextable.size && lex.lextable.table[i + cur].lexema != LEX_RETURN) {
                        cur++;
                    }

                    // Если нет возвращаемого значения, выбрасываем ошибку
                    if (i + cur < lex.lextable.size && lex.lextable.table[i + cur].lexema == LEX_RETURN) {
                        throw ERROR_THROW_IN(709, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn);
                    }
                }
                else {
                    while (i + cur < lex.lextable.size && lex.lextable.table[i + cur].lexema != LEX_RETURN) {
                        cur++;
                    }

                    if (i + cur == lex.lextable.size) {
                        throw ERROR_THROW_IN(700, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
                    }

                    // Если после return идет неправильный тип данных, выбрасываем ошибку
                    if (i + cur < lex.lextable.size && (lex.lextable.table[i + cur + 1].lexema == LEX_ID ||
                        lex.lextable.table[i + cur + 1].lexema == LEX_LITERAL) &&
                        lex.idtable.table[lex.lextable.table[i + cur + 1].idxTI].idtype != IT::FUNCTION &&
                        lex.idtable.table[lex.lextable.table[i + cur + 1].idxTI].iddatatype != returnType) {
                        throw ERROR_THROW_IN(700, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn);
                    }
                }
            }
        }

        // Второй цикл для обработки вызовов функций и их параметров
        for (int i = 0; i < lex.lextable.size; i++) {
            if (lex.lextable.table[i].lexema == LEX_ID &&
                lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::FUNCTION &&
                lex.lextable.table[i - 1].lexema == LEX_FUNCTION)
            {
                IT::IDDATATYPE* ids = new IT::IDDATATYPE[16]; 
                int idsSize = 0;
                int funcPos = lex.idtable.table[lex.lextable.table[i].idxTI].idxfirstLE; 

                while (lex.lextable.table[funcPos + 1].lexema != LEX_RIGHTTHESIS)
                {
                    if (lex.lextable.table[funcPos + 1].lexema == LEX_ID || lex.lextable.table[funcPos + 1].lexema == LEX_LITERAL) {
                        ids[idsSize] = lex.idtable.table[lex.lextable.table[funcPos + 1].idxTI].iddatatype;
                        idsSize++;
                    }
                    funcPos++;

                    if (idsSize == 16) {
                        throw ERROR_THROW_IN(705, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
                    }
                }
            }

            // Проверяем для статических функций 
            if (lex.lextable.table[i].lexema == LEX_ID &&
                lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::STATIC_FUNCTION)
            {
                int cur = 2;
                IT::IDDATATYPE dt = IT::USINT;
                if (strcmp(lex.idtable.table[lex.lextable.table[i].idxTI].id, "StrCmp") == 0) {
                    dt = IT::TEXT; 
                }
                int numberOfParams = 0; 
                while (lex.lextable.table[i + cur].lexema != LEX_RIGHTTHESIS) {
                    if (lex.lextable.table[i + cur].lexema == LEX_ID || lex.lextable.table[i + cur].lexema == LEX_LITERAL) {
                        if (lex.idtable.table[lex.lextable.table[i + cur].idxTI].iddatatype == dt) {
                            numberOfParams++;
                        }
                        else {
                            throw ERROR_THROW_IN(713, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
                        }
                    }
                    cur++;
                }
                if (numberOfParams != 2) {
                    throw ERROR_THROW_IN(713, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
                }
            }

            // Проверка на вызовы функций с правильным количеством и типами параметров
            if (lex.lextable.table[i].lexema == LEX_ID &&
                lex.idtable.table[lex.lextable.table[i].idxTI].idtype == IT::FUNCTION &&
                lex.lextable.table[i - 1].lexema != LEX_FUNCTION)
            {
                IT::IDDATATYPE* ids = new IT::IDDATATYPE[16];
                int idsSize = 0; 
                int funcPos = lex.idtable.table[lex.lextable.table[i].idxTI].idxfirstLE;

                if (lex.lextable.table[i + 1].lexema != LEX_LEFTTHESIS) {
                    throw ERROR_THROW_IN(706, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
                }

                while (lex.lextable.table[funcPos + 1].lexema != LEX_RIGHTTHESIS)
                {
                    if (lex.lextable.table[funcPos + 1].lexema == LEX_ID ||
                        lex.lextable.table[funcPos + 1].lexema == LEX_LITERAL) {
                        ids[idsSize] = lex.idtable.table[lex.lextable.table[funcPos + 1].idxTI].iddatatype;
                        idsSize++;
                    }
                    funcPos++;

                    if (idsSize == 16) {
                        throw ERROR_THROW_IN(705, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
                    }
                }

                int cur = 1;
                int paramCount = 0;
                while (lex.lextable.table[i + cur].lexema != LEX_RIGHTTHESIS) {
                    if (lex.lextable.table[i + cur].lexema == LEX_ID ||
                        lex.lextable.table[i + cur].lexema == LEX_LITERAL) {
                        if (lex.idtable.table[lex.lextable.table[i + cur].idxTI].iddatatype != ids[paramCount]) {
                            throw ERROR_THROW_IN(702, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn);
                        }
                        paramCount++;
                    }
                    cur++;
                }

                if (paramCount != idsSize) {
                    throw ERROR_THROW_IN(701, lex.lextable.table[i + cur].sn, lex.lextable.table[i + cur].cn);
                }
                i += cur; 
                delete[] ids; 
            }
        }

        // Финальная проверка на присваивание функций
        for (int i = 0; i < lex.lextable.size; i++) {
            if (lex.lextable.table[i].lexema == LEX_ASSIGNMENT &&
                lex.lextable.table[i - 1].lexema == LEX_ID &&
                lex.lextable.table[i + 1].lexema == LEX_ID &&
                lex.idtable.table[lex.lextable.table[i + 1].idxTI].idtype == IT::FUNCTION) {

                int funcIdx = lex.lextable.table[i + 1].idxTI;
                IT::IDDATATYPE returnType = lex.idtable.table[funcIdx].iddatatype; 

                int destIdx = lex.lextable.table[i - 1].idxTI; 
                IT::IDDATATYPE destType = lex.idtable.table[destIdx].iddatatype; 

                if (returnType != destType) {
                    throw ERROR_THROW_IN(707, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
                }
            }
        }
    }

    void literals(MFST::LEX lex) {
        for (int i = 0; i < lex.idtable.size; i++) {
            if (lex.idtable.table[i].idtype == IT::LITERAL && lex.idtable.table[i].iddatatype == IT::USINT) {
                unsigned long value = lex.idtable.table[i].value.vusint;
                if (value < 0 || value >= 4294967295) {
                    throw ERROR_THROW_IN(712, lex.lextable.table[lex.idtable.table[i].idxfirstLE].sn, lex.lextable.table[lex.idtable.table[i].idxfirstLE].cn);
                }
            }
        }
    }


	void ifs(MFST::LEX lex) {
		for (int i = 0; i < lex.lextable.size; i++) {
			if (lex.lextable.table[i].lexema == LEX_IF) {
 				int cur = 2;
				int counter = 0;
				bool isFunc = false;
				IT::IDDATATYPE idDataTypes[2]{};
				while (lex.lextable.table[i + cur].lexema != LEX_RIGHTTHESIS || isFunc) {
					if (lex.lextable.table[i + cur].lexema == LEX_ID || lex.lextable.table[i + cur].lexema == LEX_LITERAL && !isFunc) {
						idDataTypes[counter] = lex.idtable.table[lex.lextable.table[i + cur].idxTI].iddatatype;
						counter++;
					}
					if (lex.lextable.table[i + cur].lexema == LEX_LEFTTHESIS) isFunc = true;
					if (lex.lextable.table[i + cur].lexema == LEX_RIGHTTHESIS) isFunc = false;
					cur++;
				}
				if (counter == 1) {
					if (idDataTypes[0] != IT::BOOLEAN) {
						throw ERROR_THROW_IN(710, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
					}
				}
				else {
					if (idDataTypes[0] != idDataTypes[1] || (idDataTypes[0] == IT::BOOLEAN || idDataTypes[1] == IT::BOOLEAN) || (idDataTypes[0] == IT::TEXT || idDataTypes[1] == IT::TEXT)) {
						throw ERROR_THROW_IN(710, lex.lextable.table[i].sn, lex.lextable.table[i].cn);
					}
				}
			}
		}
		for (int i = 0; i < lex.idtable.size; i++) {
			if (lex.idtable.table[i].iddatatype == IT::HALLOW && lex.idtable.table[i].idtype != IT::FUNCTION) {
				throw ERROR_THROW_IN(708, lex.idtable.table[i].idxfirstLE, lex.idtable.table[i].idxfirstLE);
			}
		}
		
	}

	bool startSA(MFST::LEX lex) {
		functions(lex);
		operands(lex);
		literals(lex);
		ifs(lex);
		return true;
	};

}
