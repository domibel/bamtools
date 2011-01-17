// ***************************************************************************
// BamMultiReader_p.h (c) 2010 Derek Barnett
// Marth Lab, Department of Biology, Boston College
// All rights reserved.
// ---------------------------------------------------------------------------
// Last modified: 17 January 2011 (DB)
// ---------------------------------------------------------------------------
// Functionality for simultaneously reading multiple BAM files
// *************************************************************************

#ifndef BAMMULTIREADER_P_H
#define BAMMULTIREADER_P_H

//  -------------
//  W A R N I N G
//  -------------
//
// This file is not part of the BamTools API.  It exists purely as an
// implementation detail. This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.

#include <api/BamMultiReader.h>
#include <string>
#include <vector>

namespace BamTools {
namespace Internal {

class IBamMultiMerger;

class BamMultiReaderPrivate {

    // constructor / destructor
    public:
        BamMultiReaderPrivate(void);
        ~BamMultiReaderPrivate(void);

    // public interface
    public:

        // file operations
        void Close(void);
        bool Open(const std::vector<std::string>& filenames,
                  bool openIndexes = true,
                  bool coreMode = false,
                  bool preferStandardIndex = false);
        bool IsIndexLoaded(void) const;
        bool Jump(int refID, int position = 0);
        void PrintFilenames(void) const;
        bool SetRegion(const BamRegion& region);
        bool Rewind(void);

        // access alignment data
        bool GetNextAlignment(BamAlignment& al);
        bool GetNextAlignmentCore(BamAlignment& al);
        bool HasOpenReaders(void);
        void SetSortOrder(const BamMultiReader::SortOrder& order);

        // access auxiliary data
        const std::string GetHeaderText(void) const;
        const int GetReferenceCount(void) const;
        const BamTools::RefVector GetReferenceData(void) const;
        const int GetReferenceID(const std::string& refName) const;

        // BAM index operations
        bool CreateIndexes(bool useStandardIndex = true);
        void SetIndexCacheMode(const BamIndex::BamIndexCacheMode mode);

    // internal methods
    private:
        IBamMultiMerger* CreateMergerForCurrentSortOrder(void) const;
        const std::string ExtractReadGroup(const std::string& headerLine) const;
        bool LoadNextAlignment(BamAlignment& al, bool coreMode);
        void SaveNextAlignment(BamTools::BamReader* reader, BamTools::BamAlignment* alignment);
        const std::vector<std::string> SplitHeaderText(const std::string& headerText) const;
        // updates our alignment cache
        void UpdateAlignments(void);
        // validates that we have a congruent set of BAM files that are aligned against the same reference sequences
        void ValidateReaders(void) const;

    // data members
    public:
        typedef std::pair<BamReader*, BamAlignment*> ReaderAlignment;
        std::vector<ReaderAlignment> m_readers;

        IBamMultiMerger* m_alignments;
        bool m_isCoreMode;
        BamMultiReader::SortOrder m_sortOrder;
};

} // namespace Internal
} // namespace BamTools

#endif // BAMMULTIREADER_P_H
