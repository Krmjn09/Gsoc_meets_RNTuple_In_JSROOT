import {
  openFile,
  TSelector,
  treeProcess,
  createHistogram,
  makeSVG,
} from "jsroot"
import { writeFileSync } from "fs"

async function main() {
  try {
    const selector = new TSelector()

    selector.addBranch("py")

    selector.hist = null
    selector.minpy = null
    selector.maxpy = null

    selector.Begin = function () {
      this.hist = createHistogram("TH1F", 10,-5,5) // 5 bins from 10 to 25
      this.hist.fFillColor = 2 // Set fill color to red
      this.hist.fFillStyle = 1001 // Set fill style to solid
      this.hist.fName = "h1"
      this.hist.fTitle = "py Distribution"
      this.hist.fXaxis.fTitle = "py"
      this.hist.fYaxis.fTitle = "Entries"
      this.hist.fXaxis.fLabelSize = 0.02
      this.hist.fYaxis.fLabelSize = 0.02
    }

    selector.Process = function () {
      const py = this.tgtobj.py
      this.hist.Fill(py)

      if (this.minpy === null || py < this.minpy) this.minpy = py
      if (this.maxpy === null || py > this.maxpy) this.maxpy = py
    }

    selector.Terminate = async function (res) {
      if (!res) {
        console.error("Error processing TTree")
        return
      }

      console.log(
        `Data range py: ${this.minpy.toFixed(2)} to ${this.maxpy.toFixed(2)}`
      )

      const svg = await makeSVG({
        object: this.hist,
        option: "hist",
        width: 800,
        height: 600,
      })

      writeFileSync("py_histogram.svg", svg)
      console.log("Histogram saved as py_histogram.svg")
    }

    const file = await openFile("https://root.cern/js/files/hsimple.root")
    const tree = await file.readObject("ntuple")
    await treeProcess(tree, selector)

    console.log(" TTree processing completed!")
  } catch (err) {
    console.error(" Error:", err.message)
  }
}

main()
