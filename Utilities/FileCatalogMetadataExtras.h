////////////////////////////////////////////////////////////////////////
// Name: FileCatalogMetadataExtras.h
//
// Purpose: This service adds user-specified per-job and per-file sam
//          metadata.  Per-job metadata is added via to art's built in
//          FileCatalogMetadata service (which service must be configured
//          for this service to function).  Per-file sam metadata is
//          added directly to art root files after they are closed.
//
//          This service does not provide any user-callable methods.
//          Rather it adds user-specified sam metadata simply by being
//          configured and constructed.
//
//          Art stores sam metadata in the form of (name, value)
//          pairs.  An arbitrary number of per-job (name, value) pairs
//          can be specified using the single fcl parameter Metadata,
//          specified as an array of strings (the length of the array
//          must be even, obviously).
//
//          Generated per-file metadata includes the following.
//
//          1.  List of runs.
//          2.  List of subruns.
//          3.  List of parent files.
//          4.  First event.
//          5.  Last event.
//          6.  Event count.
//          7.  Start time (time when file was opened).
//          8.  End time (time when file was closed).
//
//          Per-file metadata is not configurable, except that it can be
//          turned on or off in its entirety.
//
//          This service also provides the ability to copy sam metadata
//          attributes from an input file to an output file.  The list
//          of attributes to be copied is specified by fcl parameter
//          CopyMetadataAttributes.
//
//          This service performs a separate, but related, function of
//          renaming output files according to a template provided via
//          an fcl parameter.  The purpose of file renaming is to
//          generate unique output file names accross many workers
//          from a single fcl configuration, which is something that
//          RootOutput module can not (yet) do.  Having unique file
//          names is a sam requirement.
//
//          The rename template may contain the following fields.
//
//          ${num}   - File sequence number (starts at 1).
//          ${num 0} - File sequence number (starts at 0, can be any non-negative integer).
//          ${bnum}  - Same as ${num}, with or without numeric argument,
//                     except expands as empty string for first file.
//          ${base}  - Input file name (not including directory, like basename).
//          ${base .root} - Input file name dropping final ".root".
//          ${dir}   - Input file directory path (up to, but not including final "/").
//          ${path}  - Input file path, similar as ${dir}/${file}.
//          ${path .root}  - Input file path dropping final ".root".
//          ${date}  - The current date (eight digits, YYYYMMDD).
//          ${time}  - The current time of day (six digits, HHMMSS).
//          ${VAR def} - Substitute value of environment variable VAR,
//                       or substitute "def" if VAR is undefined.
//
//          FCL parameters:
//
//          Metadata       - Array containing string (name, value) pairs.
//          GeneratePerFileMetadata - Boolean controls whether to generate
//                                    per-file metadata.
//          CopyMetadataAttributes - Array of strings containing metadata
//                                   attributes to copy from input file.
//          RenameTemplate - File rename template (do not rename if blank).
//          RenameOverwrite - Overwrite existing file if true (otherwise
//                            print a warning and don't rename).
//          
//
// Created: 26-Apr-2013,  H. Greenlee
//
////////////////////////////////////////////////////////////////////////
#ifndef FILECATALOGMETADATAMICROBOONE_H
#define FILECATALOGMETADATAMICROBOONE_H

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Principal/RunPrincipal.h"
#include "art/Framework/Principal/SubRunPrincipal.h"
#include "art/Framework/Principal/Event.h"

namespace util{

  class FileCatalogMetadataExtras
  {
  public:

    FileCatalogMetadataExtras(fhicl::ParameterSet const& pset, art::ActivityRegistry& reg);
    ~FileCatalogMetadataExtras();
      
    void reconfigure(fhicl::ParameterSet const& p);

    // Embedded per-file metadata struct.

    struct PerFileMetadata
    {
      std::set<art::RunNumber_t> fRunNumbers;
      std::set<art::SubRunNumber_t> fSubRunNumbers;
      std::set<std::string> fParents;
      art::EventNumber_t fFirstEvent;
      art::EventNumber_t fLastEvent;
      unsigned int fEventCount;
      time_t fStartTime;
      time_t fEndTime;
      std::multimap<std::string, std::string> fNVPairs;

      // Default constructor.

      PerFileMetadata() : 
	fFirstEvent(0), fLastEvent(0), fEventCount(0), fStartTime(0), fEndTime(0) {}

      // Method to convert per-file metadata to a collection of name-value pairs.

      void fillMetadata(std::multimap<std::string, std::string>& md);
    };

  private:

    // Callbacks.

    void postBeginJob();
    void postEndJob();
    void postOpenFile(std::string const& fn);
    void postCloseFile();
    void preEvent(art::Event const& ev);
    void postEvent(art::Event const& ev);

    // Pseudo-callbacks.  These are not called via the art
    // ActivityRegistry.  We generate fake callbacks ourselves by
    // monitoring root files.

    void postOpenOutputFile(std::string const& fn);
    void postCloseOutputFile(art::OutputFileInfo const& finfo);

    // Private member functions.

    static bool isArtFile(std::string const& fn);
    void checkOutputFiles();   // This method generate fake callbacks.
    void addPerFileMetadata(std::string const& fn);
    void renameOutputFile(std::string const& fn);
    std::string expandTemplate() const;

    // Data members.

    // Fcl parameters.

    std::multimap<std::string, std::string> fPerJobMetadata;   // (Name, value) pairs.
    bool fGeneratePerFileMetadata;                     // Per-file metadata flag.
    std::vector<std::string> fCopyMetadataAttributes;  // Metadata to copy from input.
    std::string fRenameTemplate;
    bool fRenameOverwrite;

    // Per-file metadata (indexed by output file name).

    std::map<std::string, PerFileMetadata> fPerFileMetadataMap;    

    // Currently open output files.

    std::vector<std::string> fOutputFiles;

    // Last seen input file.

    std::string fLastInputFile;

    // Output file count (only count art files).

    unsigned int fOutputFileCount;

  }; // class FileCatalogMetadataExtras

} //namespace utils

DECLARE_ART_SERVICE(util::FileCatalogMetadataExtras, LEGACY)

#endif
