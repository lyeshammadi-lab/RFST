#include <boost/test/unit_test.hpp>
#include <gnuradio/rfst_ula/adaptive_doa_ula.h>

BOOST_AUTO_TEST_CASE(t1)
{
    // Test minimal — vérifie juste que la factory existe
    auto blk = gr::rfst_ula::adaptive_doa_ula::make(0.05f, 1e-5f, 0.0f);
    BOOST_TEST(blk != nullptr);
}
