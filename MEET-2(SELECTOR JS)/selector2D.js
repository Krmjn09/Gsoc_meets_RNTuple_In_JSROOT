import {
  openFile,
  TSelector,
  treeProcess,
  createHistogram,
  makeSVG,
} from "jsroot"
import { writeFileSync } from "fs"

async function main() {
  const selector = new TSelector()
  selector.addBranch("px")
  selector.addBranch("py")

  selector.Begin = function () {
    this.binsX = 40
    this.xMin = -10
    this.xMax = 10
    this.binsY = 40
    this.yMin = -10
    this.yMax = 10
    this.counts = Array.from(
      { length: this.binsX },
      () => new Uint32Array(this.binsY)
    )
    this.dx = (this.xMax - this.xMin) / this.binsX
    this.dy = (this.yMax - this.yMin) / this.binsY

    this.rawX = []
    this.rawY = []
  }

  selector.Process = function () {
    const px = this.tgtobj.px
    const py = this.tgtobj.py
    if (typeof px !== "number" || typeof py !== "number") {
      console.warn("Invalid px or py:", px, py)
      return
    }
    const x = px - py
    const y = px + py

    this.rawX.push(x)
    this.rawY.push(y)

    const i = Math.floor((x - this.xMin) / this.dx)
    const j = Math.floor((y - this.yMin) / this.dy)
    if (i >= 0 && i < this.binsX && j >= 0 && j < this.binsY) {
      this.counts[i][j]++
    }
  }

  selector.Terminate = async function (res) {
    if (!res) {
      console.error("Error processing TTree")
      return
    }

    const minX = Math.min(...this.rawX)
    const maxX = Math.max(...this.rawX)
    const minY = Math.min(...this.rawY)
    const maxY = Math.max(...this.rawY)
    console.log(
      `Raw data ranges - x (px-py): [${minX}, ${maxX}], y (px+py): [${minY}, ${maxY}]`
    )
    console.log(`Total points: ${this.rawX.length}`)

    console.log("Sample points:")
    for (let i = 0; i < 5 && i < this.rawX.length; i++) {
      console.log(
        `Point ${i}: (px-py) = ${this.rawX[i]}, (px+py) = ${this.rawY[i]}`
      )
    }

    // --- 1) Heatmap ---
    {
      const hist = createHistogram("TH2F", this.binsX, this.binsY)
      hist.fName = "h2_expr"
      hist.fTitle = "(px+py) vs (px-py) Heatmap"
      hist.fXaxis.fTitle = "px - py"
      hist.fYaxis.fTitle = "px + py"
      hist.fXaxis.fXmin = this.xMin
      hist.fXaxis.fXmax = this.xMax
      hist.fYaxis.fXmin = this.yMin
      hist.fYaxis.fXmax = this.yMax

      for (let i = 0; i < this.binsX; i++) {
        for (let j = 0; j < this.binsY; j++) {
          const binContent = this.counts[i][j]
          if (binContent > 0) {
            hist.Fill(
              this.xMin + (i + 0.5) * this.dx,
              this.yMin + (j + 0.5) * this.dy,
              binContent
            )
          }
        }
      }

      const svg = await makeSVG({
        object: hist,
        option: "colz",
        width: 800,
        height: 600,
       
      })
      writeFileSync("expr_heatmap.svg", svg)
      console.log("expr_heatmap.svg written")
    }

    // --- 2) Scatter Plot ---
    {
      const histScatter = createHistogram("TH2F", this.binsX, this.binsY)
      histScatter.fName = "h2_scatter_expr"
      histScatter.fTitle = "(px+py) vs (px-py) Scatter Plot"
      histScatter.fXaxis.fTitle = "px - py"
      histScatter.fYaxis.fTitle = "px + py"
      histScatter.fXaxis.fXmin = this.xMin
      histScatter.fXaxis.fXmax = this.xMax
      histScatter.fYaxis.fXmin = this.yMin
      histScatter.fYaxis.fXmax = this.yMax

      for (let i = 0; i < this.rawX.length; i++) {
        histScatter.Fill(this.rawX[i], this.rawY[i])
      }

      const svg = await makeSVG({
        object: histScatter,
        option: "SCAT",
        width: 800,
        height: 600,
      
      })
      writeFileSync("expr_scatter.svg", svg)
      console.log("expr_scatter.svg written")
    }

   
  }

  try {
    const file = await openFile("https://root.cern/js/files/hsimple.root")
    const tree = await file.readObject("ntuple")
    if (!tree) {
      console.error("Failed to load ntuple from file")
      return
    }
    await treeProcess(tree, selector)
    console.log("All done!")
  } catch (err) {
    console.error("Fatal Error:", err.message)
  }
}

main()
