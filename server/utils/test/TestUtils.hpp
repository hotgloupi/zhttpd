
#ifndef __TestUtils_HPP__
# define __TestUtils_HPP__

# include <vector>

# include "utils/MemoryPool.hpp"
# include "utils/Timer.hpp"
# include "utils/SmartPtr.hpp"
# include "utils/Path.hpp"
# include "utils/Regex.hpp"
# include "unitester/Suite.hpp"
# include "utils/StatsManager.hpp"

class TestUtils : public Test::Suite<TestUtils>
{
    private:
        void _timerTest()
        {
            ZHTTPD::Timer t;
            std::cout << "Just created" << " : elapsedTime = " << t.getElapsedTime() << std::endl;
            for (size_t i = 1 ; i < 10 ; ++i)
            {
                ZHTTPD::Timer::sleep(20);
                std::cout << "sleep(20) * " << i << " : elapsedTime = " << t.getElapsedTime() << std::endl;
            }
            t.reset();
            std::cout << "reset : elapsedTime = " << t.getElapsedTime() << std::endl;
        }

        void _smartPtrTest()
        {
            {
                ZHTTPD::SmartPtr<int> ptr1 = new int(50);
                this->assert_test(*ptr1 == 50, "*ptr1 == 50");
                ZHTTPD::SmartPtr<int> ptr2 = ptr1;
                ZHTTPD::SmartPtr<int> ptr3 = ptr2;
                this->assert_test(*ptr3 == 50, "*ptr3 == 50");
            }

            {
                ZHTTPD::SmartPtr<int> ptr1 = new int(12);
                {
                    ZHTTPD::SmartPtr<int> ptr2 = ptr1;
                    ++(*ptr2);
                    ptr2 = new int(1);
                    this->assert_test(*ptr1 == 13, "*ptr1 == 13");
                }
            }
        }

        void _pathTestUnix()
        {
            {
                ZHTTPD::Path path("/home/ipc/workspace/toto.gif");
                ZHTTPD::Path subpath("/home/ipc/workspace/truc/toto/../coucou.txt");
                ZHTTPD::Path dir("/home/ipc/workspace");
                ZHTTPD::Path relative("toto/");
                this->assert_test(subpath.isSubdirectory(path), "subDirectory failed");
                this->assert_test(path.isSubdirectory(subpath) == false, "subDirectory should have failed, but succeed");
                this->assert_test(subpath.getPath() == "/home/ipc/workspace/truc/coucou.txt", "wrong path: " + subpath.getPath('-'));
                this->assert_test(subpath.getBasePath() == "/home/ipc/workspace/truc/", "wrong base path: " + subpath.getBasePath());
                this->assert_test(path.getFileExtension() == "gif", "Wrong extension: should be 'gif', but '" + path.getFileExtension() + "' found");
                this->assert_test(path.getFileName() == "toto.gif", "Wrong filename: should be 'toto.gif', but '" + path.getFileName() + "' found");
                this->assert_test(path.getType() == ZHTTPD::NOT_A_FILE, "wrong type. should be 'NOT_A_FILE'");
                this->assert_test(subpath.getType() == ZHTTPD::FILE, "wrong type. should be 'FILE'");
                this->assert_test(dir.getType() == ZHTTPD::DIRECTORY, "wrong type. should be 'DIRECTORY'");
                this->assert_test(dir.getDirectoryFiles().empty() == false, "getDirectoryFiles failed");
                this->assert_test(dir.isInDirectory("truc"), "isInDirectory failed");
                this->assert_test(dir.isInDirectory("trucdeouf") == false, "isInDirectory should have failed");
                //this->assert_test(relative.getAbsolutePath() == "", relative.getAbsolutePath());
            }
        }

        void _pathTestWindows()
        {
            {
                ZHTTPD::Path path("C:\\Users\\ipc\\toto.gif");
                ZHTTPD::Path subpath("C:\\Users\\ipc\\truc\\toto\\..\\coucou.txt");
                ZHTTPD::Path dir("C:\\Users\\");
                this->assert_test(subpath.isSubdirectory(path), "subDirectory failed");
                this->assert_test(path.isSubdirectory(subpath) == false, "subDirectory should have failed, but succeed");
                this->assert_test(subpath.getPath('-') == "C:-Users-ipc-truc-coucou.txt", "wrong path: " + subpath.getPath('-'));
                this->assert_test(subpath.getBasePath() == "C:\\Users\\ipc\\truc\\", "wrong base path: " + subpath.getBasePath());
                this->assert_test(path.getFileExtension() == "gif", "Wrong extension: should be 'gif', but '" + path.getFileExtension() + "' found");
                this->assert_test(path.getFileName() == "toto.gif", "Wrong filename: should be 'toto.gif', but '" + path.getFileName() + "' found");
                this->assert_test(path.getType() == ZHTTPD::NOT_A_FILE, "wrong type. should be 'NONE'");
                this->assert_test(path.exists(), path.getPath() + " does not exist");
                this->assert_test(subpath.exists() == false, subpath.getPath() + " exists");
                this->assert_test(dir.getType() == ZHTTPD::DIRECTORY, dir.getPath() + " is not a directory");
                this->assert_test(path.getType() == ZHTTPD::FILE, path.getPath() + " is not a file");
                this->assert_test(subpath.getType() == ZHTTPD::NOT_A_FILE, subpath.getPath() + " exists");
            }
        }

        void _regexTest()
        {
            ZHTTPD::Regex r;
            this->assert_test(r.match("a*b", "achjb"), "match failed 1");
            this->assert_test(!r.match("a*b", "achj"), "match should have failed 1");
            this->assert_test(r.match("a*b", "acbhjb"), "match failed 2");
            this->assert_test(r.match("a*b", "ab"), "match failed 3");
            this->assert_test(r.match("a?b", "aqb"), "match failed 4");
            this->assert_test(r.match("a******b", "ab"), "match failed 5");
            this->assert_test(r.match("a*?b", "aqb"), "match failed 6");
            this->assert_test(!r.match("a*?b", "ab"), "match should have failed 2");
            this->assert_test(!r.match("?", ""), "match should have failed 3");
            this->assert_test(r.match("*", ""), "match failed 7");
        }

        void _pouleTest1()
        {
            unsigned int n = 1000000;
            ZHTTPD::Timer t;
            ZHTTPD::MemoryPool<long double> a(n);
            std::vector<long double*> ptrs;
            std::vector<long double*>::iterator it;
            std::vector<long double*>::iterator itEnd;
            ptrs.reserve(n);

            for (unsigned int j = 0; j < 3; ++j)
            {
                std::cout << "PASS " << j + 1 << std::endl << "===================" << std::endl;

                std::cout << "*** Poule allocator ***" << std::endl;
                std::cout << "\tAllocating " << n << " long double with poule" << std::endl;
                t.reset();
                for (unsigned int i = 0; i < n; ++i)
                    ptrs.push_back(a.allocate());
                std::cout << "\tDone in " << t.getElapsedTime() << " ms" << std::endl;

                std::cout << "\tFreeing " << n << " long double with poule" << std::endl;
                t.reset();
                it = ptrs.begin();
                itEnd = ptrs.end();
                for (; it != itEnd; ++it)
                    a.release(*it);
                ptrs.clear();
                std::cout << "\tDone in " << t.getElapsedTime() << " ms" << std::endl;

                std::cout << "*** new/delete ***" << std::endl;
                std::cout << "\tAllocating " << n << " long double with new" << std::endl;
                t.reset();
                for (unsigned int i = 0; i < n; ++i)
                    ptrs.push_back(new long double);
                std::cout << "\tDone in " << t.getElapsedTime() << " ms" << std::endl;

                std::cout << "\tFreeing " << n << " long double with delete" << std::endl;
                t.reset();
                it = ptrs.begin();
                itEnd = ptrs.end();
                for (; it != itEnd; ++it)
                    delete *it;
                ptrs.clear();
                std::cout << "\tDone in " << t.getElapsedTime() << " ms" << std::endl;
            }

            this->assert_test(ZHTTPD::StatsManager::getInstance()->getMemory() == n * sizeof(long double), "ZHTTPD::StatsManager::getInstance()->getMemory() == lol");
            std::cout << "Destroying the poule allocator..." << std::endl;
        }

        void _pouleTest2()
        {
            this->assert_test(ZHTTPD::StatsManager::getInstance()->getMemory() == 0, "ZHTTPD::StatsManager::getInstance()->getMemory() == 0");
        }

        class hey
        {
            public:
                hey()
                {
                    std::cout << "hey ctor" << std::endl;
                }

                ~hey()
                {
                    std::cout << "hey dtor" << std::endl;
                }
        };

        void _pouleTest3()
        {
            ZHTTPD::MemoryPool<hey> m(10);
            hey* h = m.allocate();
            m.release(h);
            m.clean();
        }

    public:
        TestUtils() : Test::Suite<TestUtils>("Utils tests")
    {
        this->addTestFunction(&TestUtils::_timerTest, "Timer tests");
        this->addTestFunction(&TestUtils::_smartPtrTest, "SmartPtr tests");
#ifdef _WIN32
        this->addTestFunction(&TestUtils::_pathTestWindows, "Path tests Windows");
#else
        this->addTestFunction(&TestUtils::_pathTestUnix, "Path tests Unix");
#endif
        this->addTestFunction(&TestUtils::_regexTest, "Regex tests");
        this->addTestFunction(&TestUtils::_pouleTest1, "PouleAllocator 1");
        this->addTestFunction(&TestUtils::_pouleTest2, "PouleAllocator 2");
        this->addTestFunction(&TestUtils::_pouleTest3, "PouleAllocator 3");
    }
};

#endif /* !__TestUtils_HPP__ */

