/*
 * telseq.h
 *
 *  Created on: 6 Sep 2013
 *      Author: zd1
 */

#ifndef TELSEQ_H_
#define TELSEQ_H_


#include <getopt.h>
#include <vector>
#include <map>
#include "Util.h"
#include "config.h"

namespace ScanParameters{

	static std::string FIELD_SEP="\t";
	const unsigned int GENOME_LENGTH_AT_TEL_GC =  1097938400;
	const unsigned int READ_LENGTH=100;
	const std::string PATTERN="TTAGGG";
	const std::string PATTERN_REVCOMP="CCCTAA";
	const float GC_LOWERBOUND = 0.4;
	const float GC_UPPERBOUND = 0.6;
	const float GC_BINSIZE = 0.02;

	const float GC_TELOMERIC_LOWERBOUND = 0.58;
	const float GC_TELOMERIC_UPPERBOUND = 0.62;

	// maximum motif counts. add 1 to include 0 count.
	const int TEL_MOTIF_N = READ_LENGTH/PATTERN.size() +1;
	const int TEL_MOTIF_CUTOFF = 7;
	const int GC_BIN_N = (int) ((GC_UPPERBOUND-GC_LOWERBOUND)/GC_BINSIZE+0.5);

	const std::string LABEL_RG="ReadGroup";
	const std::string LABEL_SAMPLE="Sample";
	const std::string LABEL_BAM="BAM";
	const std::string LABEL_TOTAL="Total";
	const std::string LABEL_MAPPED="Mapped";
	const std::string LABEL_DUP="Duplicates";
	const std::string LABEL_TEL="TEL";
	const std::string LABEL_GC="GC";
	const std::string LABEL_LEN="LENGH_ESTIMATE";

	const std::string SCAN_FILE_SUFFIX = "bamscan";

};

struct ScanResults
{
	ScanResults() { setDefaults(); }

    // Set reasonable default values for the qc filters
    void setDefaults()
    {
		telcounts = std::vector<int>(ScanParameters::TEL_MOTIF_N,0);
		gccounts = std::vector<int>(ScanParameters::GC_BIN_N,0);
        numTotal = 0;
        numMapped = 0;
        numDuplicates = 0;
        telLenEstimate = 0;
    }
	std::string sample;
	std::string bam;
	std::vector<int> telcounts;
	std::vector<int> gccounts;
	unsigned int numTotal;
	unsigned int numMapped;
	unsigned int numDuplicates;
	double telLenEstimate;

};

// headers for the output

struct Headers{

	Headers(){setHeaders();}
	void setHeaders(){
		headers.push_back(ScanParameters::LABEL_RG);
		headers.push_back(ScanParameters::LABEL_SAMPLE);
		headers.push_back(ScanParameters::LABEL_TOTAL);
		headers.push_back(ScanParameters::LABEL_MAPPED);
		headers.push_back(ScanParameters::LABEL_DUP);
		headers.push_back(ScanParameters::LABEL_LEN);

		for(int i=0;i < ScanParameters::TEL_MOTIF_N;i++){
			std::string h = ScanParameters::LABEL_TEL + NumberToString(i);
			headers.push_back(h);
		}
		for(int i=0;i<ScanParameters::GC_BIN_N;i++){
			std::string h = ScanParameters::LABEL_GC + NumberToString(i);
			headers.push_back(h);
		}
	}
	std::vector<std::string> headers;

};


void parseScanOptions(int argc, char** argv);
double calcGC(const std::string& seq);
int countMotif(std::string &read, std::string pattern, std::string pattern_revcomp);
int printresults(std::map <std::string, ScanResults>);
double calcTelLength(ScanResults results);
int scanBam();


#endif /* TELSEQ_H_ */