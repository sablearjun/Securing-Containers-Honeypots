import loadingGif from '../assets/images/loader-table.svg'

const LoadGraph = () => {
  return (
    <div style={{display:"flex",width:"100%",height:"100%",justifyContent:"center",alignItems:"center"}}>
        <img src={loadingGif} alt="loading..." />
    </div>
  )
}

export default LoadGraph;