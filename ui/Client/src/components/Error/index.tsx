interface ErrorParams{
    message:string
}

export const Error = (params:ErrorParams) => {
  return (
    <div className="col d-flex justify-content-center align-items-center" style={{height:"100%",color:'grey', fontWeight:600, fontSize:'2rem'}}>
        <p style={{fontSize:"2rem"}}>{params.message}</p>
    </div>
  )
}
