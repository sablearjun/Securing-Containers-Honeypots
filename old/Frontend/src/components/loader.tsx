import React from 'react'
import loadingGif from '../assets/images/Infinity_loader.svg'

const Loader = () => {
  return (
    <div className='loader-section'>
        <img src={loadingGif} alt="loading..." />
    </div>
  )
}

export default Loader;
