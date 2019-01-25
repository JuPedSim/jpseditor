class Jpseditor < Formula
  desc "A graphical user interface to create the geometry of a scenario simulated by JuPedSim"
  homepage "https://www.jupedsim.org/jpseditor/"
#   head "https://github.com/JuPedSim/jpseditor.git" :branch => "version087"
  head "https://gitlab.version.fz-juelich.de/jupedsim/jpseditor.git" :branch => "version087"

  depends_on "cmake" => :build
  depends_on "qt"

  def install
    Dir.pwd
    Dir.mkdir "build"
    Dir.chdir "build"
    system "cmake", "..", *std_cmake_args
    system "make", "install"
    puts ""
    puts "------------------------------------"
    puts "JPSeditor installed in /tmp"
    puts "open /tmp/JPSeditor.app"
    puts "------------------------------------"
  end
end
