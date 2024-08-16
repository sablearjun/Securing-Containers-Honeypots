import { FC, PropsWithChildren, useRef, useState } from "react";
import { validateSession, renewToken } from "../../apis/requests";
import Loader from "../loader";
import { logout } from "../../helpers";

interface AppContainerProps {}

const AppContainer: FC<PropsWithChildren<AppContainerProps>> = ({
  children,
}: PropsWithChildren<AppContainerProps>) => {
  const [loading, setLoading] = useState<boolean>(false);
  const [validSession, setValidSession] = useState<boolean>(true);

  try {
    const authenticatedUser = sessionStorage.getItem("authenticatedUser");
    if (!authenticatedUser) {
      window.location.href = "/";
      return <div>Not Authenticated!</div>;
    }
    const { isAuthenticated } = JSON.parse(authenticatedUser);

    if (!isAuthenticated) {
      window.location.href = "/";
      return <div>Not Authenticated!</div>;
    }

    validateSession().then((session) => {
      if (!session) {
        setValidSession(false);
      }
    });

    // try {
    //   validateSession().then((session)=>{
    //     if (!session) {
    //       let req={
    //         "username":localStorage.getItem("Username"),
    //         "renew_access_token":localStorage.getItem("Renew_access_token")
    //     }
    //     renewToken(req)
    //       .then( (res) => {
    //         if(res.non_field_errors){
    //           console.log(res.non_field_errors[0])
    //         }else{
    //           let x = res.token;
    //           localStorage.setItem('Token', x)
    //           localStorage.setItem('Renew_access_token', res.renew_access_token)
    //           localStorage.setItem('Username', res.user_data['username'])
    //           if(x)
    //           {
    //             sessionStorage.setItem(
    //               "authenticatedUser",
    //               JSON.stringify({ isAuthenticated: true, username: localStorage.getItem("Username") })
    //             );
    //             window.location.reload();
    //           }
    //         }
    //       })
    //       .catch(err => {
    //         console.log(err)
    //       })
    //     }
    //   })
    // } catch (error) {
    //   console.log(error)
    // }
  } catch (e) {
    window.location.href = "/";
    return <div>Not Authenticated!</div>;
  }

  return (
    <>
      {loading ? (
        <Loader />
      ) : (
        <div className="vh-100 bg-light">
          <div className="bg-primary w-100" style={{ height: "6px" }}>
            <div className="vstack gap-3 px-5 py-3">
              {validSession ? (
                children
              ) : (
                <div className="d-flex flex-column justify-content-center align-items-center">
                  <h2 className="fs-3 mt-4">Session expired! </h2>
                  <button className="btn btn-danger" onClick={() => logout()}>
                    Re-Join
                  </button>
                </div>
              )}
            </div>
          </div>
        </div>
      )}
    </>
  );
};

export default AppContainer;
