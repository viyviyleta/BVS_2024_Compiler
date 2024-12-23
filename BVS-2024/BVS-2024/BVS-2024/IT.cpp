#include "IT.h"
#include "Error.h"
#include <string.h>

namespace IT {
	IdTable Create() {
		IdTable* idtable = new IdTable();
		idtable->size = 0;
		return *idtable;
	}

	void Add(IdTable& idtable, Entry entry) {
		if (idtable.size + 1 > TI_MAXSIZE) {
			throw ERROR_THROW(117);
		}
		bool isUnique = true;
		if (entry.idtype != IT::LITERAL) {
			for (int i = 0; i < idtable.table.size(); i++) {
				if (strcmp(idtable.table[i].id, entry.id) == 0) {
					isUnique = false;
					break;
				}
			}
		}
		if (isUnique) {
			idtable.table.push_back(entry);
			idtable.size++;
		}
		else {
			throw ERROR_THROW(118);
		}
	}

	Entry GetEntry(IdTable& idtable, int n) {
		return idtable.table[n];
	}

	int IsId(IdTable& idtable, char id[ID_MAXSIZE]) {
		for (int i = 0; i < idtable.size; i++) {
			if (strcmp(idtable.table[i].id, id) == 0) {
				return i;
			}
		}
		return TI_NULLIDX;
	}
	
	void AddFunctionStaticLib(IdTable& idtable) {
		Entry powFunction(0, (char*)"Pow", IDDATATYPE(1), IDTYPE(5));
		Entry sumFunction(1, (char*)"Sum", IDDATATYPE(1), IDTYPE(5));
		Entry strcmpFunction(2, (char*)"StrCmp", IDDATATYPE(1), IDTYPE(5));
		Add(idtable, powFunction);
		Add(idtable, sumFunction);
		Add(idtable, strcmpFunction);
	}

	void Delete(IdTable& idtable) {
		delete& idtable;
	}
}