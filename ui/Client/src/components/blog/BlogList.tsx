import React, { useState, useEffect } from "react";
import { getData } from "../../apis/requests";
import { GET_BLOGS_URL } from "../../apis/urls";
import loaderForTable from "../../assets/images/loader-table.svg";
import { Link } from "react-router-dom";

const BlogList = () => {
  const [blogs, setBlogs] = useState<any>(undefined);

  useEffect(() => {
    if (blogs === undefined) {
      getData(GET_BLOGS_URL, setBlogs);
    }
  }, []);

  return (
    <>
      <div>
        <div
          id="blogList"
          className="d-flex flex-wrap justify-content-center align-items-center gap-8 h-100"
        >
          <div className="container">
            <br />
            <br />
            <h1 className="fw-bold text-primary text-center outfit fs-1 mt-3">
              Recent News
            </h1>
            <div className="d-flex justify-content-center gap-7 my-7">
              <div className="d-flex flex-column justify-content-center align-items-center">
                <div className="d-flex flex-column justify-content-center">
                  {blogs === undefined || blogs?.data?.length === 0?(
                    <>
                        <div className="card-body px-6">
                          <p className="text-center text-secondary fw-bold">
                            No blogs found.
                          </p>
                        </div>
                    </>
                  ) : (
                    <>
                      {blogs?.data?.map((blog: any, index: any) => {
                        return (
                          <div className="card flex-1 my-4 p-7 mx-3" key={index}>
                            <h2 className="text-primary fs-4 lh-base mt-1">
                              <Link to="/blog" state={blog} className="text-decoration-none">
                              {blog.name}</Link>
                            </h2>
                            <p className="fw-mid text-black-50 fs-4 lh-base text-justify mt-1">
                              {blog.content.slice(0, 100)}<br/>
                              <Link to="/blog" state={blog}>
                              <button className="btn btn-primary">Read more</button>
                              </Link>
                            </p>
                          </div>
                        );
                      })}
                    </>
                  )}
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </>
  );
};

export default BlogList;
