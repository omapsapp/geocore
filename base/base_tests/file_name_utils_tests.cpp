#include "testing/testing.hpp"

#include "base/file_name_utils.hpp"

#include <string>

UNIT_TEST(FileName_Smoke)
{
  std::string name = "/Users/xxx/Documents/test.test";
  TEST_EQUAL(base::GetFileExtension(name), ".test", ());
  base::GetNameFromFullPath(name);
  TEST_EQUAL(name, "test.test", ());
  base::GetNameFromFullPath(name);
  TEST_EQUAL(name, "test.test", ());
  base::GetNameWithoutExt(name);
  TEST_EQUAL(name, "test", ());

  name = "C:\\My Documents\\test.test";
  TEST_EQUAL(base::GetFileExtension(name), ".test", ());
  base::GetNameFromFullPath(name);
  TEST_EQUAL(name, "test.test", ());
  base::GetNameWithoutExt(name);
  TEST_EQUAL(name, "test", ());

  name = "/";
  TEST_EQUAL(base::GetFileExtension(name), std::string(), ());
  base::GetNameFromFullPath(name);
  TEST(name.empty(), ());

  name = "C:\\";
  TEST_EQUAL(base::GetFileExtension(name), std::string(), ());
  base::GetNameFromFullPath(name);
  TEST(name.empty(), ());

  name = "../test";
  TEST_EQUAL(base::GetFileExtension(name), std::string(), ());
  base::GetNameFromFullPath(name);
  TEST_EQUAL(name, "test", ());
  base::GetNameWithoutExt(name);
  TEST_EQUAL(name, "test", ());
}

UNIT_TEST(FileName_GetDirectory)
{
  TEST_EQUAL("/tmp", base::GetDirectory("/tmp/evil\\file"), ());
  TEST_EQUAL(".", base::GetDirectory("evil\\file"), ());

  TEST_EQUAL("/", base::GetDirectory("/somefile.txt"), ());

  TEST_EQUAL("/", base::GetDirectory("////somefile"), ());
  TEST_EQUAL("a/b", base::GetDirectory("a/b///somefile.txt"), ());

  TEST_EQUAL("/a/b", base::GetDirectory("/a/b/c"), ());
  TEST_EQUAL("/a/b", base::GetDirectory("/a/b/c.txt"), ());

  TEST_EQUAL(".", base::GetDirectory("somefile.txt"), ());
  TEST_EQUAL(".", base::GetDirectory("somefile"), ());
}

UNIT_TEST(FilePath_Slash)
{
  TEST_EQUAL("/", base::AddSlashIfNeeded(""), ());
  TEST_EQUAL("/", base::AddSlashIfNeeded("/"), ());
  TEST_EQUAL("./", base::AddSlashIfNeeded("."), ());
  TEST_EQUAL("data/", base::AddSlashIfNeeded("data"), ());
  TEST_EQUAL("data/", base::AddSlashIfNeeded("data/"), ());
  TEST_EQUAL("/data/", base::AddSlashIfNeeded("/data"), ());
  TEST_EQUAL("/data/", base::AddSlashIfNeeded("/data/"), ());
  TEST_EQUAL("../../data/", base::AddSlashIfNeeded("../../data"), ());
  TEST_EQUAL("../../data/", base::AddSlashIfNeeded("../../data/"), ());
}

UNIT_TEST(FilePath_Join)
{
  TEST_EQUAL("geocore/strings.txt", base::JoinPath("geocore", "strings.txt"), ());
  TEST_EQUAL("geocore/strings.txt", base::JoinPath("geocore/", "strings.txt"), ());
  TEST_EQUAL("../../geocore/strings.txt", base::JoinPath("..", "..", "geocore", "strings.txt"), ());
  TEST_EQUAL("../../geocore/strings.txt", base::JoinPath("../", "..", "geocore/", "strings.txt"), ());
}
