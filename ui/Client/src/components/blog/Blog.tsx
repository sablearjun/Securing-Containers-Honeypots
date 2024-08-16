import React, { useEffect } from "react";
import { useLocation } from "react-router-dom";
import ControlledCarousel from "../carousel";

const Blog = () => {

  const location = useLocation();
  let edata: any = location.state; // as blog
  useEffect(()=>{
    window.scrollTo(0, 0);
  },[])
  return (
    <>
      {edata === undefined?    <>
      <div className="container">
        <div className="d-flex justify-content-center align-items-center">
          <div className="card">
            <h3> No Blog data to view. Refer the list of blogs first: </h3>
          </div>
        </div>
      </div>
      </>:<>
      <div>
        <div
          id="about"
          className="d-flex flex-wrap justify-content-center align-items-center gap-8 h-100"
        >
          <div className="card mx-4 p-1">
          <div className="row m-0 p-1">
            <div>
              <h1 className="fw-bold text-primary outfit fs-1 text-start my-0 my-md-3 ps-4">
                {edata.name}
              </h1>
            </div>
            <div className="row m-0">
              <div className="col-12">
                <div className="d-flex flex-column justify-content-start mx-3 my-4">
                  <p className="fw-mid text-black-50 text-justify fs-4 lh-base mt-1">
                   {edata.content}
                  </p>
                </div>
              </div>
              {edata.image? <>
                <div className="col-12">
                <div className="d-flex flex-column justify-content-between align-items-start mx-3 my-4">
                  <div className="col-12 col-md-8">
                  <img className="w-100" src={edata.image} alt={edata.name}/>
                  </div>
                </div>
              </div>
              </>:<></>}
            </div>
          </div>
          </div>
        </div>
      </div>
      </>}
    </>
  );
};

export default Blog;
