#pragma once

#include "generator/generate_info.hpp"
#include "generator/intermediate_data.hpp"
#include "generator/osm_o5m_source.hpp"
#include "generator/osm_xml_source.hpp"
#include "generator/translator_interface.hpp"

#include "coding/parse_xml.hpp"

#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>

struct OsmElement;
class FeatureParams;

namespace generator
{
class SourceReader
{
  struct Deleter
  {
    bool m_needDelete;
    Deleter(bool needDelete = true) : m_needDelete(needDelete) {}
    void operator()(std::istream * s) const
    {
      if (m_needDelete)
        delete s;
    }
  };

  std::unique_ptr<std::istream, Deleter> m_file;

public:
  SourceReader();
  explicit SourceReader(std::string const & filename);
  explicit SourceReader(std::istream & stream);

  uint64_t Read(char * buffer, uint64_t bufferSize);
};

bool GenerateIntermediateData(feature::GenerateInfo const & info);

void ProcessOsmElementsFromO5M(SourceReader & stream, std::function<void(OsmElement &&)> processor);
void ProcessOsmElementsFromXML(SourceReader & stream, std::function<void(OsmElement &&)> processor);

class ProcessorOsmElementsInterface
{
public:
  virtual ~ProcessorOsmElementsInterface() = default;

  virtual bool TryRead(OsmElement & element) = 0;
};

class ProcessorOsmElementsFromO5M : public ProcessorOsmElementsInterface
{
public:
  explicit ProcessorOsmElementsFromO5M(SourceReader & stream,
                                       size_t taskCount = 1, size_t taskId = 0,
                                       size_t chunkSize = 1);

  // ProcessorOsmElementsInterface overrides:
  bool TryRead(OsmElement & element) override;

  size_t ChunkSize() const noexcept { return m_chunkSize; }

private:
  SourceReader & m_stream;
  osm::O5MSource m_dataset;
  size_t const m_taskCount;
  size_t const m_taskId;
  size_t const m_chunkSize;
  uint64_t m_elementCounter{0};
  osm::O5MSource::Iterator m_pos;

  bool Read(OsmElement & element);
};

class ProcessorOsmElementsFromXml : public ProcessorOsmElementsInterface
{
public:
  explicit ProcessorOsmElementsFromXml(SourceReader & stream);

  // ProcessorOsmElementsInterface overrides:
  bool TryRead(OsmElement & element) override;

private:
  bool TryReadFromQueue(OsmElement & element);

  XMLSource m_xmlSource;
  XMLSequenceParser<SourceReader, XMLSource> m_parser;
  std::queue<OsmElement> m_queue;
};
}  // namespace generator
