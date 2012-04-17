#!/usr/bin/env python
import optparse, os, subprocess, sys

def runInternal(suffix, args, out=sys.stdout):
    if os.name != "posix":
        suffix += ".exe"
    env = os.environ
    root = os.path.abspath(os.path.dirname(__file__))
    env["TEXTTEST_HOME"] = root
    env["ACTIVITYGEN_BINARY"] = os.path.join(root, "..", "bin", "activitygenInt" + suffix)
    env["DFROUTER_BINARY"] = os.path.join(root, "..", "bin", "dfrouterInt" + suffix)
    env["DUAROUTER_BINARY"] = os.path.join(root, "..", "bin", "duarouterInt" + suffix)
    env["JTRROUTER_BINARY"] = os.path.join(root, "..", "bin", "jtrrouterInt" + suffix)
    env["NETCONVERT_BINARY"] = os.path.join(root, "..", "bin", "netconvertInt" + suffix)
    env["NETGEN_BINARY"] = os.path.join(root, "..", "bin", "netgenInt" + suffix)
    env["OD2TRIPS_BINARY"] = os.path.join(root, "..", "bin", "od2tripsInt" + suffix)
    env["SUMO_BINARY"] = os.path.join(root, "..", "bin", "meso" + suffix)
    env["POLYCONVERT_BINARY"] = os.path.join(root, "..", "bin", "polyconvertInt" + suffix)
    env["GUISIM_BINARY"] = os.path.join(root, "..", "bin", "meso-gui" + suffix)
    subprocess.call("texttest.py %s -a sumo.internal,sumo.meso,complex.meso,duarouter.astar,duarouter.chrouter,netconvert.internal" % args,
                    stdout=out, stderr=out, shell=True)

if __name__ == "__main__":
    optParser = optparse.OptionParser()
    optParser.add_option("-s", "--suffix", default="", help="suffix to the fileprefix")
    (options, args) = optParser.parse_args()
    runInternal(options.suffix, " ".join(["-" + a for a in args]))
