#include "Smith.h"

#include <codecvt>
#include <sstream>

void Smith::updateAlternative()
{
	string file = SVG::SVG_element::to_ascii(readFinishedSVG("Z:/aprn/aab.svg"));

	size_t nextText = 0, i = 0;
	nextText = file.find("<text", i);
	do {
		size_t closing = min(file.find("</text", nextText), file.find("/>", nextText));
		if (closing == string::npos) {
			break;
		}
		size_t wordStart = file.find("id=\"", nextText);
		size_t wordEnd = file.find("\"", wordStart + 4);

		string word, clue;
		if (wordStart != string::npos && wordEnd != string::npos) {
			for (i = wordStart + 4; i < wordEnd; i++) {
				word.push_back(file[i]);
			}
		}
		size_t lineStart = nextText;
		size_t lineEnd = nextText;
		int lineBr = 0;
		while (1) {
			lineStart = file.find("<tspan", lineEnd);
			if (lineStart == string::npos || lineStart >= closing)break;
			lineStart = file.find(">", lineStart);
			if (lineStart == string::npos || lineStart >= closing)break;
			lineEnd = file.find("</tspan", lineStart);
			if (lineEnd == string::npos || lineEnd >= closing)break;
			//cout << lineStart << " " << lineEnd << endl;
			for (i = lineStart + 1; i < lineEnd; i++)clue.push_back(file[i]);
			clue.push_back('\n');
			lineBr++;
		}
		while(!clue.empty() && clue.back() == '\n')clue.pop_back();

		if (word.empty()) {
			nextText = file.find("<text", closing);
			continue;
		}
		if (clue.empty()) {
			nextText = file.find("<text", closing);
			continue;
		}
		replace(clue, "&quot;", "\"");
		if (alternatives.count(word)) {
			if (!alternatives[word].count(clue)) {
				alternatives[word][clue] = lineBr;
			}
		}
		else {
			alternatives[word] = map<string, int> { {clue, lineBr} };
		}
		//cout << word << endl << clue << endl << endl;

		nextText = file.find("<text", closing);
	} while (nextText != string::npos);
}

void Smith::printAlternativesToFile()
{
	ofstream fout("altDict.txt");
	for (auto & i : alternatives) {
		for (auto & j : i.second) {
			fout << i.first << endl;
			fout << j.first << endl;
			fout << "!!!" << endl;
		}
	}
	fout.close();
}

void Smith::printTxtAnswers()
{
	ofstream fout(string("Z:\\Cross\\" + cross.name + ".txt").c_str());
	vector<string> NameClues;
	for (size_t i = 0; i < cross.areas.size(); i++) {
		string temp = dict.explanationDict[cross.areas[i].to_string()], NameClue;
		bool secondName = false;
		for (size_t j = 0; j < temp.size(); j++) {
			if (temp[j] == '/') {
				j++;
				while (temp[j] != '/') {
					if(!secondName)NameClue.push_back(temp[j++]);
					else ++j;
				}
				secondName = true;
			}
		}
		if (NameClue.back() == ' ')NameClue.pop_back();
		NameClues.push_back(NameClue);
	}
	fout << "���������: ";
	
	for (int i = 0; i < words.size(); i++) {
		if (!words[i].hor)continue;
		string areaWord = cross.areas[words[i].areaInd].to_string();
		string unedited = (dict.dirtyDict.count(areaWord) ? dict.dirtyDict[areaWord] : areaWord);
		for (int j = 0; j < unedited.size(); j++) {
			if (uc(unedited[j]) > crossword::cyrillicA - 33 && uc(unedited[j]) < crossword::cyrillicA + 32) {
				if (uc(unedited[j]) >= crossword::cyrillicA && uc(unedited[j]) < crossword::cyrillicA + 32)unedited[j] -= 32;
				break;
			}
		}
		fout << unedited;
		if (!NameClues[words[i].areaInd].empty())fout << " (" + NameClues[words[i].areaInd] + ")";
		fout << ". ";
	}

	auto wordsTemp = words;
	sort(wordsTemp.begin(), wordsTemp.end(), [](duma& A, duma& B) -> bool{
		if (A.j == B.j)return A.i < B.i;
		return A.j < B.j;
	});

	fout << endl << "�������: ";
	
	for (int i = 0; i < wordsTemp.size(); i++) {
		if (wordsTemp[i].hor)continue;
		string areaWord = cross.areas[wordsTemp[i].areaInd].to_string();
		string unedited = (dict.dirtyDict.count(areaWord) ? dict.dirtyDict[areaWord] : areaWord);
		for (int j = 0; j < unedited.size(); j++) {
			if (uc(unedited[j]) > crossword::cyrillicA - 33 && uc(unedited[j]) < crossword::cyrillicA + 32) {
				if (uc(unedited[j]) >= crossword::cyrillicA && uc(unedited[j]) < crossword::cyrillicA + 32)unedited[j] -= 32;
				break;
			}
		}
		fout << unedited;
		if (!NameClues[wordsTemp[i].areaInd].empty())fout << " (" + NameClues[wordsTemp[i].areaInd] + ")";
		fout << ". ";
	}
	fout.close();

}

wstring Smith::readFinishedSVG(string filename)
{
	std::wifstream wif(filename);
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstringstream wss;
	wss << wif.rdbuf();
	return wss.str();
}
