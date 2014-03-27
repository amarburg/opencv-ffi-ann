
require 'minitest'
require 'minitest/autorun'

require 'opencv-rice-ann'

class TestDescriptors < Minitest::Test
  include CVRice
  include CVRice::ANN

  def test_descriptors
    m = Mat.new

    desc = Descriptors.new m

    assert_equal 0, desc.length

  end

end
