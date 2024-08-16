import React, { useEffect, useState, useReducer } from "react";
import { deleteMe, getDataReducer } from "../apis/requests";
import {
  GET_USER_URL,
  GET_GROUPS_URL,
  GET_USER_LIST_URL,
  CREATE_GROUP_URL,
  UPDATE_GROUP_MEMBER,
  DELETE_GROUP,
  DELETE_USER_URL,
  GET_BLOGS_URL,
  BLOG_ADD_URL,
  BLOG_DELETE_URL,
  BLOG_UPDATE_URL,
  UPDATE_USER_URL,
} from "../apis/urls";
import { getData, updateMe, doSignUp } from "../apis/requests";
import Checkbox from "@mui/material/Checkbox";
import { Event, newTeam } from "../types";
import Button from "react-bootstrap/Button";
import Button1 from "@mui/material/Button";
import Modal from "react-bootstrap/Modal";
import Alert from "@mui/material/Alert";
import Chip from "@mui/material/Chip";
import { Tree, TreeNode } from "react-organizational-chart";
import Box from "@mui/material/Box";
import Modal1 from "@mui/material/Modal";
import TextField from "@mui/material/TextField";
import Autocomplete from "@mui/material/Autocomplete";
import { LineChart, Line, XAxis, YAxis, Tooltip, Legend } from "recharts";
import Email from "./email";
import DataPurge from "./datapurge";
import SecondaryTable from "../components/SecondaryTable";
import { INITIAL_STATE, reducerFunc } from "../apis/requests/reducer";
import Snackbar from "@mui/material/Snackbar";
import { wait } from "@testing-library/user-event/dist/utils";
import Dropdown from "react-bootstrap/Dropdown";
import DropdownButton from "react-bootstrap/DropdownButton";
import BlogList from "../components/blog/BlogList";
import { timeConverter } from "../helpers";
import { ToastContainer, toast } from 'react-toastify';

const DEFAULT_INDEX = 100000000; // Can't be undefined or <0 as it throws errors

const data = [
  {
    name: "Monday",
    assigned: 4000,
    completed: 2400,
  },
  {
    name: "Tuesday",
    assigned: 3000,
    completed: 1398,
  },
  {
    name: "Wednesday",
    assigned: 2000,
    completed: 9800,
  },
  {
    name: "Thursday",
    assigned: 2780,
    completed: 3908,
  },
  {
    name: "Friday",
    assigned: 1890,
    completed: 4800,
  },
  {
    name: "Saturday",
    assigned: 2390,
    completed: 3800,
  },
  {
    name: "Sunday",
    assigned: 3490,
    completed: 4300,
  },
];

interface userInfo {
  username: string;
  password: string;
  passwordConfirm: string;
  email: string;
  name: string;
  role: string;
}

const Settings = () => {
  const [user, setUser] = useState<any>(undefined);
  const [groupList, setGroupList] = useState<any>(undefined);
  const [userList, setUserList] = useReducer(reducerFunc, INITIAL_STATE);
  const [selectedGroup, setSelectedGroup] = useState<any>(undefined);
  const [newTeam, setNewTeam] = useState<newTeam>({
    name: "",
    member_ids: [],
    permissions: [],
  });
  const [open, setOpen] = React.useState(false);
  const [openAddUser, setOpenAddUser] = React.useState(false);
  const [openDeleteUser, setOpenDeleteUser] = React.useState(false);
  const [openAddBlog, setOpenAddBlog] = React.useState(false);
  const [editBlog, setEditBlog] = React.useState(false);
  const handleOpen = () => setOpen(true);
  const handleClose = () => {
    setOpen(false);
    setNewTeam((prev: any) => ({
      ...prev,
      name: "",
      permissions: [],
      member_ids: [],
    }));
  };
  const handleChange = (name:string) => () => {
    if(name === '') {
      toast.error('Blog name is mandatory.');
      return false;
    }
     if(!name.match('[a-zA-Z\s&\d]'))
     {
      toast.error('Enter valid Blog name.');
      return false;
     }
  }
  const [openSnakBar, setOpenSnakBar] = React.useState(false);
  const [alertData, setAlertData] = React.useState<any>(undefined);
  const [isMenuOpen, setIsMenuOpen] = useState(false);
  const [activeGroup, setActiveGroup] = useState(DEFAULT_INDEX);
  const [isMenuOpenBlog, setIsMenuOpenBlog] = useState(false);
  const [activeBlog, setActiveBlog] = useState(DEFAULT_INDEX);
  const [blogsList, setBlogsList] = useState<any>(undefined);
  const [isMenuOpenUser, setIsMenuOpenUser] = useState(false);
  const [activeUser, setActiveUser] = useState(DEFAULT_INDEX);
  const [editUser, setEditUser] = React.useState(false);
  //const [userList, setUserList] = useState<any>(undefined);
  //const [activeUser, setActiveBlog] = useState(DEFAULT_INDEX);
  const toggleOpen = (Id: any) => {
    setIsMenuOpen(!isMenuOpen);
    setActiveGroup(Id);
  };

  const toggleOpenBlog = (Id: any) => {
    setIsMenuOpenBlog(!isMenuOpenBlog);
    setActiveBlog(Id);
  };
  const toggleOpenUser = (Id: any) => {
    setIsMenuOpenUser(!isMenuOpenUser);
    setActiveUser(Id);
  };

  const handleClick = () => {
    setOpenSnakBar(true);
  };

  const handleClickAway = (
    event?: React.SyntheticEvent | Event,
    reason?: string
  ) => {
    if (reason === "clickaway") {
      setIsMenuOpen(false);
      return;
    }

    setOpenSnakBar(false);
  };

  const handleMembers = (action: string, user: any, group: any) => {
    let payload: any = { team_id: group.id, member_ids: "", append: false };
    if (action == "DELETE") {
      console.log(user.id);
      payload.member_ids = group.member_ids
        .split(",")
        .filter((i: any) => Number(i) !== Number(user.id))
        .join(",");
      // console.log("payload",payload);
      updateMe(payload, UPDATE_GROUP_MEMBER, setAlertData);
      getData(GET_GROUPS_URL, setGroupList);
    }
  };

  const handleTeamCreation = () => {
    updateMe(newTeam, CREATE_GROUP_URL, setAlertData);
    wait(2);
    getData(GET_GROUPS_URL, setGroupList);
    handleClose();
  };

  const handleTeamDeletion = (id: any) => {
    deleteMe({ id: id }, DELETE_GROUP, setAlertData);
    wait(2);
    getData(GET_GROUPS_URL, setGroupList);
    // getData(GET_GROUPS_URL, setGroupList);
    // window.location.href='/settings';
  };

  useEffect(() => {
    getData(GET_USER_URL, setUser);
    getData(GET_GROUPS_URL, setGroupList);
    getDataReducer(GET_USER_LIST_URL, setUserList);
    getData(GET_BLOGS_URL, setBlogsList);
  }, []);

  const AddGroupModal = () => {
    const style = {
      position: "absolute" as "absolute",
      top: "50%",
      left: "50%",
      transform: "translate(-50%, -50%)",
      display: "flex",
      width: "40vw",
      // height:"30vh",
      bgcolor: "background.paper",
      // border: '2px solid #000',
      boxShadow: 24,
      p: 2,
    };

    return (

      <div>
        <Modal1
          open={open}
          onClose={handleClose}
          aria-labelledby="modal-modal-title"
          aria-describedby="modal-modal-description"
        >
          <Box sx={style}>
            <div className="modal-body">
              <form>
                <div className="row">
                  <div className="col-6">
                    <div className="form-floating mb-4">
                      <input
                        type="text"
                        className="form-control"
                        id="floatingInput"
                        placeholder="Notes"
                        onChange={(e: any) => {
                          newTeam.name = e.target.value;
                        }}
                        required
                      />
                      <label htmlFor="floatingInput">Group Name</label>
                    </div>
                  </div>
                  <div className="col-6">
                    <Autocomplete
                      style={{ marginBottom: "1rem" }}
                      multiple
                      aria-required
                      id="tags-outlined"
                      options={[
                        { name: "permission level 1" },
                        { name: "permission level 2" },
                        { name: "permission level 3" },
                        { name: "permission level 4" },
                      ]}
                      getOptionLabel={(option: any) => option.name}
                      filterSelectedOptions
                      onChange={(event, value) => {
                        // setNewTeam((prev:any)=>({...prev,Permissions:value}))
                        newTeam.permissions = value.map((permissions: any) => {
                          return permissions.name;
                        });
                      }}
                      renderInput={(params) => (
                        <TextField
                          {...params}
                          label="Select Group Permissions"
                          placeholder="permission"
                        />
                      )}
                    />
                  </div>
                </div>
                {userList !== undefined ? (
                  <Autocomplete
                    multiple
                    aria-required
                    id="tags-outlined"
                    options={userList?.Data?.data}
                    getOptionLabel={(option: any) => option.username}
                    filterSelectedOptions
                    onChange={(event, value) => {
                      // setNewTeam((prev:any)=>({...prev,member_ids:value}))
                      newTeam.member_ids = value.map((members: any) => {
                        return members.id;
                      });
                    }}
                    renderInput={(params) => (
                      <TextField
                        {...params}
                        label="Select Team Members"
                        placeholder="Employee"
                      />
                    )}
                  />
                ) : (
                  <></>
                )}
              </form>
              <div className="d-flex flex-row-reverse mt-5 gap-5">
                <Button1
                  color="secondary"
                  variant="outlined"
                  onClick={() => {
                    handleTeamCreation();
                    wait(2);
                    getData(GET_GROUPS_URL, setGroupList);
                  }}
                >
                  Create Team
                </Button1>
                <Button1
                  variant="outlined"
                  color="error"
                  onClick={() => {
                    handleClose();
                  }}
                >
                  Cancel
                </Button1>
              </div>
            </div>
          </Box>
        </Modal1>
      </div>
    );
  };
  const EditUserModal = () => {
    const [userInfo, setUserInfo] = useState<any>({
      id: userList?.Data?.data?.[activeUser]?.id,
      username: userList?.Data?.data?.[activeUser]?.username,
      password: "",
      passwordConfirm: "" ,
      email:userList?.Data?.data?.[activeUser]?.email ,
      name: userList?.Data?.data?.[activeUser]?.name,
      role: userList?.Data?.data?.[activeUser]?.role,
    });

    
    const [errorMessage, setErrorMessage] = useState<string>('');
    const [textClass, setTextClass] = useState<string>('text-danger');

    function successResponse(res: any) {
      setAlertData(res);
      getDataReducer(GET_USER_LIST_URL, setUserList);
    }
    
    const callUpdateAPI = () => {
      // setIsLoading(true)
      let req = {
        id:userInfo.id,
        username: userInfo.username,
        password: userInfo.password,
        passwordConfirm: userInfo.passwordConfirm,
        email: userInfo.email,
        name: userInfo.name,
        role: userInfo.role,
      };

      console.log(req);
      updateMe(req ,UPDATE_USER_URL ,setAlertData);
       
    };

    function validateEmail(email: string) {
      var re = /\S+@\S+\.\S+/;
      return re.test(email);
    }

    let validator = (info: userInfo) => {
      let res = true;
      Object.entries(info).map(([ky, val]) => {
        if (res) {
          res = val !== "";
          if (ky === "email") {
            res = validateEmail(info.email);
          }
          if (ky === "passwordConfirm") {
            res = info.password === info.passwordConfirm;
          }
        }
      });
      return res;
    };

    function isValidPassword(password: string) {
      // Check if the password is at least 6 characters and at most 25 characters
      if (password.length < 6 || password.length > 25) {
        // setErrorMessage("Password length out of bounds");
        // setTextClass('text-danger');
        return false;
      }

      // Check for at least one uppercase letter
      if (!/[A-Z]/.test(password)) {
        // setErrorMessage("Weak Password");
        // setTextClass('text-danger');
        return false;
      }

      // Check for at least one lowercase letter
      if (!/[a-z]/.test(password)) {
        // setErrorMessage("Weak Password");
        // setTextClass('text-danger');
        return false;
      }

      // Check for at least one special character (you can modify the regex to include your preferred special characters)
      if (!/[@#$%^&+=]/.test(password)) {
        // setErrorMessage("Weak Password");
        // setTextClass('text-danger');
        return false;
      }

      // Check for at least one number
      if (!/[0-9]/.test(password)) {
        // setErrorMessage("Weak Password");
        // setTextClass('text-danger');
        return false;
      }

      // setErrorMessage("Strong Password");
      // // setTextClass('text-success');
      return true;
    }

    return (
      <>
       <ToastContainer />
        <Modal
          show={editUser}
          onHide={() => {
            setEditUser(false);
          }}
        >
          <Modal.Header closeButton>
            <Modal.Title>{"Edit User"}</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <form>
            <div className="form-floating mb-3">
                <input
                  type="text"
                  className={`form-control ${
                    userInfo.name === "" ? "border-danger" : ""
                  }`}
                  id="floatingName"
                  placeholder="Name"
                  value={userInfo.name}
                  onChange={(e) =>
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      name: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingName">Name</label>
              </div>
              <div className="form-floating mb-3">
                <input
                  type="email"
                  className={`form-control ${
                    userInfo.email === "" || !validateEmail(userInfo.email)
                      ? "border-danger"
                      : ""
                  }`}
                  id="floatingEmail"
                  placeholder="email"
                  value={userInfo.email}
                  onChange={(e) =>
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      email: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingEmail">Email</label>
              </div>
              <div className="form-floating mb-3">
                <select
                  name="role"
                  id="floatingRole"
                  className={`form-control ${
                    userInfo.role === "" ? "border-danger" : ""
                  }`}
                  value={userInfo.role}
                  onChange={(e) =>
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      role: e.target.value,
                    }))
                  }
                >
                  <option value="ADMINISTRATOR">Administrator</option>
                  <option value="STANDARD">Standard</option>
                </select>
                <label htmlFor="floatingRole">Role</label>
              </div>
              <div className="form-floating mb-4">
                <input
                  type="text"
                  className={`form-control ${
                    userInfo.username === "" ? "border-danger" : ""
                  }`}
                  id="floatingInput"
                  placeholder="username"
                  value={userInfo.username}
                  onChange={(e) =>
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      username: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingInput">Username</label>
              </div>
              <div className="form-floating d-flex mb-3">
                <input
                  type="password"
                  className={`form-control ${
                    userInfo.password === "" ||
                    !isValidPassword(userInfo.password)
                      ? "border-danger"
                      : ""
                  }`}
                  id="floatingPassword"
                  placeholder="Password"
                  value={userInfo.password}
                  onChange={(e) =>{
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      password: e.target.value,
                    }))
                    if (e.target.value.length < 6 || e.target.value.length > 25) {
                      setErrorMessage("Password length out of bounds");
                      setTextClass('text-danger');
                   }else if(isValidPassword(e.target.value)){
                      setErrorMessage("Strong Password");
                      setTextClass('text-success');
                   }else{
                    setErrorMessage("Weak Password");
                    setTextClass('text-danger');
                   }
                }}
                />
                <i
                  title="Password length should be >=6 and <=25. (One uppercase, one lowercase, one special character and one number atleast required)"
                  style={{ background: "#f8f9fa" }}
                  className="p-2 fa fa-info ms-2 bg-secondary d-flex justify-content-center align-items-center"
                ></i>
                <label htmlFor="floatingPassword">Password</label>
              </div>
              <div className="my-1">
                {errorMessage === '' ? null : <span className={`${textClass}`} >{errorMessage}</span>}
              </div>
              <div className="form-floating mb-3">
                <input
                  type="password"
                  className={`form-control ${
                    userInfo.passwordConfirm === "" ? "border-danger" : ""
                  }`}
                  id="floatingConfirmPassword"
                  placeholder="Repeat Password"
                  value={userInfo.passwordConfirm}
                  onChange={(e) =>
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      passwordConfirm: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingConfirmPassword">
                  Confirm Password
                </label>
              </div>
            </form>
          </Modal.Body>
          <Modal.Footer>
            <Button
              variant="secondary"
              onClick={() => {
                setErrorMessage('');
                setTextClass('');
                setEditUser(false);
                //setOpenAddUser(false);
              }}
            >
              Close
            </Button>
            <Button
              variant="primary"
              onClick={() => {
                if (validator(userInfo) && isValidPassword(userInfo.password)) {
                  callUpdateAPI();
                  setEditUser(false);
                  //setOpenAddUser(false);
                } else {
                  setAlertData({
                    status: "error",
                    message: "Please enter valid data",
                  });
                  setEditUser(false);
                  //setOpenAddUser(false);
                }
                setErrorMessage('');
                setTextClass('');
              }}
            >
              Update
            </Button>
          </Modal.Footer>
        </Modal>
      </>
    );
  };


  const AddUserModal = () => {
    const [userInfo, setUserInfo] = useState<userInfo>({
      username: "",
      password: "",
      passwordConfirm: "",
      email: "",
      name: "",
      role: "STANDARD",
    });

    
    const [errorMessage, setErrorMessage] = useState<string>('');
    const [textClass, setTextClass] = useState<string>('text-danger');

    function successResponse(res: any) {
      setAlertData(res);
      getDataReducer(GET_USER_LIST_URL, setUserList);
    }

    const callSignupAPI = () => {
      // setIsLoading(true)
      let req = {
        username: userInfo.username,
        password: userInfo.password,
        passwordConfirm: userInfo.passwordConfirm,
        email: userInfo.email,
        name: userInfo.name,
        role: userInfo.role,
      };

      console.log(req);
      doSignUp(req)
        .then((res) => {
          res?.message === "User successfully created."
            ? successResponse(res)
            : handleClick();
        })
        .catch((err) => {
          handleClick();
        });
    };

    function validateEmail(email: string) {
      var re = /\S+@\S+\.\S+/;
      return re.test(email);
    }

    let validator = (info: userInfo) => {
      let res = true;
      Object.entries(info).map(([ky, val]) => {
        if (res) {
          res = val !== "";
          if (ky === "email") {
            res = validateEmail(info.email);
          }
          if (ky === "passwordConfirm") {
            res = info.password === info.passwordConfirm;
          }
        }
      });
      return res;
    };

    function isValidPassword(password: string) {
      // Check if the password is at least 6 characters and at most 25 characters
      if (password.length < 6 || password.length > 25) {
        // setErrorMessage("Password length out of bounds");
        // setTextClass('text-danger');
        return false;
      }

      // Check for at least one uppercase letter
      if (!/[A-Z]/.test(password)) {
        // setErrorMessage("Weak Password");
        // setTextClass('text-danger');
        return false;
      }

      // Check for at least one lowercase letter
      if (!/[a-z]/.test(password)) {
        // setErrorMessage("Weak Password");
        // setTextClass('text-danger');
        return false;
      }

      // Check for at least one special character (you can modify the regex to include your preferred special characters)
      if (!/[@#$%^&+=]/.test(password)) {
        // setErrorMessage("Weak Password");
        // setTextClass('text-danger');
        return false;
      }

      // Check for at least one number
      if (!/[0-9]/.test(password)) {
        // setErrorMessage("Weak Password");
        // setTextClass('text-danger');
        return false;
      }

      // setErrorMessage("Strong Password");
      // // setTextClass('text-success');
      return true;
    }

    return (
      <>
       <ToastContainer />
        <Modal
          show={openAddUser}
          onHide={() => {
            setOpenAddUser(false);
          }}
        >
          <Modal.Header closeButton>
            <Modal.Title>{"Add User"}</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <form>
            <div className="form-floating mb-3">
                <input
                  type="text"
                  className={`form-control ${
                    userInfo.name === "" ? "border-danger" : ""
                  }`}
                  id="floatingName"
                  placeholder="Name"
                  value={userInfo.name}
                  onChange={(e) =>
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      name: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingName">Name</label>
              </div>
              <div className="form-floating mb-3">
                <input
                  type="email"
                  className={`form-control ${
                    userInfo.email === "" || !validateEmail(userInfo.email)
                      ? "border-danger"
                      : ""
                  }`}
                  id="floatingEmail"
                  placeholder="email"
                  value={userInfo.email}
                  onChange={(e) =>
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      email: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingEmail">Email</label>
              </div>
              <div className="form-floating mb-3">
                <select
                  name="role"
                  id="floatingRole"
                  className={`form-control ${
                    userInfo.role === "" ? "border-danger" : ""
                  }`}
                  value={userInfo.role}
                  onChange={(e) =>
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      role: e.target.value,
                    }))
                  }
                >
                  <option value="ADMINISTRATOR">Administrator</option>
                  <option value="STANDARD">Standard</option>
                </select>
                <label htmlFor="floatingRole">Role</label>
              </div>
              <div className="form-floating mb-4">
                <input
                  type="text"
                  className={`form-control ${
                    userInfo.username === "" ? "border-danger" : ""
                  }`}
                  id="floatingInput"
                  placeholder="username"
                  value={userInfo.username}
                  onChange={(e) =>
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      username: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingInput">Username</label>
              </div>
              <div className="form-floating d-flex mb-3">
                <input
                  type="password"
                  className={`form-control ${
                    userInfo.password === "" ||
                    !isValidPassword(userInfo.password)
                      ? "border-danger"
                      : ""
                  }`}
                  id="floatingPassword"
                  placeholder="Password"
                  value={userInfo.password}
                  onChange={(e) =>{
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      password: e.target.value,
                    }))
                    if (e.target.value.length < 6 || e.target.value.length > 25) {
                      setErrorMessage("Password length out of bounds");
                      setTextClass('text-danger');
                   }else if(isValidPassword(e.target.value)){
                      setErrorMessage("Strong Password");
                      setTextClass('text-success');
                   }else{
                    setErrorMessage("Weak Password");
                    setTextClass('text-danger');
                   }
                }}
                />
                <i
                  title="Password length should be >=6 and <=25. (One uppercase, one lowercase, one special character and one number atleast required)"
                  style={{ background: "#f8f9fa" }}
                  className="p-2 fa fa-info ms-2 bg-secondary d-flex justify-content-center align-items-center"
                ></i>
                <label htmlFor="floatingPassword">Password</label>
              </div>
              <div className="my-1">
                {errorMessage === '' ? null : <span className={`${textClass}`} >{errorMessage}</span>}
              </div>
              <div className="form-floating mb-3">
                <input
                  type="password"
                  className={`form-control ${
                    userInfo.passwordConfirm === "" ? "border-danger" : ""
                  }`}
                  id="floatingConfirmPassword"
                  placeholder="Repeat Password"
                  value={userInfo.passwordConfirm}
                  onChange={(e) =>
                    setUserInfo((prev: userInfo) => ({
                      ...prev,
                      passwordConfirm: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingConfirmPassword">
                  Confirm Password
                </label>
              </div>
            </form>
          </Modal.Body>
          <Modal.Footer>
            <Button
              variant="secondary"
              onClick={() => {
                setErrorMessage('');
                setTextClass('');
                setOpenAddUser(false);
              }}
            >
              Close
            </Button>
            <Button
              variant="primary"
              onClick={() => {
                if (validator(userInfo) && isValidPassword(userInfo.password)) {
                  callSignupAPI();
                  setOpenAddUser(false);
                } else {
                  setAlertData({
                    status: "error",
                    message: "Please enter valid data",
                  });
                  setOpenAddUser(false);
                }
                setErrorMessage('');
                setTextClass('');
              }}
            >
              Register
            </Button>
          </Modal.Footer>
        </Modal>
      </>
    );
  };

  

  const AddBlogModal = () => {
    const [blog, setBlog] = useState<any>({
      name: "",
      content: "",
      image: "",
      links: "",
    });

    return (
      <>
       <ToastContainer />
        <Modal
          show={openAddBlog}
          onHide={() => {
            setOpenAddBlog(false);
          }}
        >
          <Modal.Header closeButton>
            <Modal.Title>"Add Blog"</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <form>
              <div className="form-floating mb-4">
                <input
                  type="text"
                  className={`form-control ${
                    blog.name === "" ? "border-danger" : ""
                  }`}
                  id="floatingInput"
                  placeholder="Title"
                  
                  pattern = "/^[A-Za-z0-9\s]+$/i"
                  value={blog.name}
                  //required onChange= {handleChange("name")}
                  onChange={(e) =>
                   setBlog((prev: any) => ({
                     ...prev,
                     name: e.target.value,
                   }))
                  }
                />
                <label htmlFor="floatingInput"> Title</label>
              </div>
              <div className="form-floating mb-4">
                <textarea
                  rows={5}
                  cols={35}
                  className={`form-control ${
                    blog.content === "" ? "border-danger" : ""
                  }`}
                  id="floatingInput"
                  placeholder="Content"
                  value={blog.content}
                  required
                  onChange={(e) =>
                    setBlog((prev: any) => ({
                      ...prev,
                      content: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingInput">Content</label>
              </div>
              <div className="form-floating mb-4">
                <input
                  type="text"
                  className={`form-control`}
                  id="floatingInput"
                  placeholder="Image Link"
                  value={blog.image}
                  onChange={(e) =>
                    setBlog((prev: any) => ({
                      ...prev,
                      image: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingInput">Image Link</label>
              </div>
              <div className="form-floating mb-4">
                <input
                  type="text"
                  className={`form-control`}
                  id="floatingInput"
                  placeholder="Links"
                  value={blog.links}
                  onChange={(e) =>
                    setBlog((prev: any) => ({
                      ...prev,
                      links: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingInput">Links</label>
              </div>
            </form>
          </Modal.Body>
          <Modal.Footer>
            <Button
              variant="secondary"
              onClick={() => {
                setOpenAddBlog(false);
                setBlog({
                  name: "",
                  content: "",
                  image: "",
                  links: "",
                });
              }}
            >
              Close
            </Button>
            <Button
              variant="primary"
              onClick={() => {
                if(blog.name === '') {
                  toast.error('Blog name is mandatory.');
                  return false;
                }
                 if(!blog.name.match('[a-zA-Z\s&\d]'))
                 {
                  toast.error('Enter valid Blog name.');
                  return false;
                 }
                //setAlertData({ message: "Insert valid Title" })
                updateMe(blog, BLOG_ADD_URL, setAlertData);
                wait(2);
                getData(GET_BLOGS_URL, setBlogsList);
                setBlog({
                  name: "",
                  content: "",
                  image: "",
                  links: "",
                });
                setOpenAddBlog(false);
              }}
            >
              Add
            </Button>
          </Modal.Footer>
        </Modal>
      </>
    );
  };

  const UpdateBlogModal = () => {
    const [blog, setBlog] = useState<any>({
      id: blogsList?.data?.[activeBlog]?.id,
      name: blogsList?.data?.[activeBlog]?.name,
      content: blogsList?.data?.[activeBlog]?.content,
      image: blogsList?.data?.[activeBlog]?.image,
      links: blogsList?.data?.[activeBlog]?.links,
    });

    return (
      <>
       <ToastContainer />
        <Modal
          // id="EditBlogsModal"
          show={editBlog}
          onHide={() => {
            setEditBlog(false);
          }}
        >
          <Modal.Header closeButton>
            <Modal.Title>{"Edit Blog"}</Modal.Title>
          </Modal.Header>
          <Modal.Body>
            <form>
              <div className="form-floating mb-4">
                <input
                  type="text"
                  className={`form-control ${
                    blog.name === "" ? "border-danger" : ""
                  }`}
                  id="floatingInput"
                  placeholder="Title"
                  value={blog.name}
                
                  required
                  onChange={(e) =>
                    setBlog((prev: any) => ({
                      ...prev,
                      name: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingInput"> Title</label>
              </div>
              <div className="form-floating mb-4">
                <textarea
                  rows={5}
                  cols={35}
                  className={`form-control ${
                    blog.content === "" ? "border-danger" : ""
                  }`}
                  id="floatingInput"
                  placeholder="Content"
                  value={blog.content}
                  
                  required
                
                
                  onChange={(e) =>
                    setBlog((prev: any) => ({
                      ...prev,
                      content: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingInput">Content</label>
              </div>
              <div className="form-floating mb-4">
                <input
                  type="text"
                  className={`form-control`}
                  id="floatingInput"
                  placeholder="Image Link"
                  value={blog.image}
                  onChange={(e) =>
                    setBlog((prev: any) => ({
                      ...prev,
                      image: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingInput">Image Link</label>
              </div>
              <div className="form-floating mb-4">
                <input
                  type="text"
                  className={`form-control`}
                  id="floatingInput"
                  placeholder="Links"
                  value={blog.links}
                  onChange={(e) =>
                    setBlog((prev: any) => ({
                      ...prev,
                      links: e.target.value,
                    }))
                  }
                />
                <label htmlFor="floatingInput">Links</label>
              </div>
            </form>
          </Modal.Body>
          <Modal.Footer>
            <Button
              variant="secondary"
              onClick={() => {
                setBlog({
                  id: "",
                  name: "",
                  content: "",
                  image: "",
                  links: "",
                });
                setEditBlog(false);
              }}
            >
              Close
            </Button>
            <Button
              variant="primary"
              onClick={() => {
                updateMe(blog, BLOG_UPDATE_URL, setAlertData);
                wait(2);
                getData(GET_BLOGS_URL, setBlogsList);
                setBlog({
                  id: "",
                  name: "",
                  content: "",
                  image: "",
                  links: "",
                });
                setEditBlog(false);
              }}
            >
              Update
            </Button>
          </Modal.Footer>
        </Modal>
      </>
    );
  };

  const DeleteUserModal = () => {
    const [deleteUser, setDeleteUser] = useState<string>("Select");

    function successResponse(res: any) {
      setAlertData(res);
      getDataReducer(GET_USER_LIST_URL, setUserList);
    }
    return (
      <>
        <Modal
          show={openDeleteUser}
          onHide={() => {
            setOpenDeleteUser(false);
          }}
        >
          <Modal.Header closeButton>
            <Modal.Title>{"Delete User"}</Modal.Title>
        
          </Modal.Header>
          <Modal.Body>
            <form>
              <div className="form-floating mb-4">
                <DropdownButton id="dropdown-basic-button" title={deleteUser}>
                  {userList ? (
                    userList?.Data?.data?.map((user: any, index: number) => (
                      <Dropdown.Item
                        key={user.id} // Use a unique key
                        onClick={() => {
                          setDeleteUser(user.username);
                        }}
                      >
                        {user.username}
                      </Dropdown.Item>
                    ))
                  ) : (
                    <></>
                  )}
                </DropdownButton>
              </div>
            </form>
          </Modal.Body>
          <Modal.Footer>
            <Button
              variant="secondary"
              onClick={() => {
                setOpenDeleteUser(false);
                setDeleteUser("Select");
              }}
            >
              Cancel
            </Button>
            <Button
              variant="danger"
              onClick={() => {
                if (deleteUser === "Select") {
                  setAlertData({ message: "Select user to be deleted" });
                } else {
                  let user = userList.Data.data.find(
                    (user: any) => user.username === deleteUser
                  );
                  deleteMe({ id: user.id }, DELETE_USER_URL, successResponse);
                  getDataReducer(GET_USER_LIST_URL, setUserList);
                  setDeleteUser("Select");
                  setOpenDeleteUser(false);
                }
              }}
            >
              Delete
            </Button>
          </Modal.Footer>
        </Modal>
        
      </>
    );
  };

  if (user !== undefined && user?.data?.[0].role === "ADMINISTRATOR") {
    return (
      <>
        <AddGroupModal />
        <AddUserModal />
        <EditUserModal/>
        <DeleteUserModal />
        <AddBlogModal />
        <UpdateBlogModal />
        {alertData !== undefined ? (
          alertData.status === "Success" ? (
            <Alert
              onClose={() => {
                setAlertData(undefined);
              }}
            >
              {alertData.message}
            </Alert>
          ) : (
            <Alert
              onClose={() => {
                setAlertData(undefined);
              }}
              severity="error"
            >
              {alertData.message}
            </Alert>
          )
        ) : (
          ""
        )}
        <div className="card card-body">
          <div className="hstack">
            {/* Sidebar for Admin Panel */}
            <div className="d-flex flex-column justify-content-start col-md-2">
              <h3 className="fw-mid my-0">Admin Panel</h3>
            </div>

            {/* Main content */}
            <div className="col-md-10">
              <div className="row">
                {/* Group */}
                <div className="col-6 col-md-3 col-lg-2 border-end">
                  <div className="row d-flex justify-content-center align-items-center">
                    <h4 className="fw-mid d-flex justify-content-center align-items-center">
                      Group
                    </h4>
                  </div>
                  <div className="row d-flex justify-content-center align-items-center">
                    <h4 className="fw-normal d-flex justify-content-center align-items-center">
                      {groupList !== undefined ? groupList?.data?.length : 0}
                    </h4>
                  </div>
                </div>

                {/* Staff */}
                <div className="col-6 col-md-3 col-lg-2 border-end">
                  <div className="row d-flex justify-content-center align-items-center">
                    <h4 className="fw-mid d-flex justify-content-center align-items-center">
                      Staff
                    </h4>
                  </div>
                  <div className="row d-flex justify-content-center align-items-center">
                    <h4 className="fw-normal d-flex justify-content-center align-items-center">
                      {userList !== undefined
                        ? userList.Data?.data?.length - 1
                        : 0}
                    </h4>
                  </div>
                </div>

                {/* Admin */}
                <div className="col-6 col-md-3 col-lg-2 mt-6 mt-md-0 border-end">
                  <div className="row d-flex justify-content-center align-items-center">
                    <h4 className="fw-mid d-flex justify-content-center align-items-center">
                      Admin
                    </h4>
                  </div>
                  <div className="row d-flex justify-content-center align-items-center">
                    <h4 className="fw-normal d-flex justify-content-center align-items-center">
                      {userList !== undefined
                        ? // userList.Data?.data?.filter(
                          //     (item: any) => item.role === "ADMINSTRATOR"
                          //   ).length
                          1
                        : 0}
                    </h4>
                  </div>
                </div>

                {/* Unassigned alerts */}
                <div className="col-6 col-md-3 col-lg-2 mt-6 mt-md-0">
                  <div className="row d-flex justify-content-center align-items-center">
                    <h4 className="fw-mid d-flex justify-content-center align-items-center">
                      Unassigned alerts
                    </h4>
                  </div>
                  <div className="row d-flex justify-content-center align-items-center">
                    <h4 className="fw-normal d-flex justify-content-center align-items-center">
                      {(1256).toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",")}
                    </h4>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
        <div className="card py-3">
          <div className="card-body px-5">
            <div className="d-flex flex-column align-items-start">
              <div className="d-flex justify-content-between align-items-center gap-3 w-100">
                <div className="d-flex justify-content-between align-items-center gap-0">
                  <ul className="nav nav-tabs justify-content-between mx-2">
                    <li className="nav-item">
                      <a
                        aria-controls="overview"
                        aria-selected="true"
                        className="nav-link px-2 active"
                        data-bs-target="#overview"
                        data-bs-toggle="tab"
                        id="overview-tab"
                        role="tab"
                        type="button"
                      >
                        Overview
                      </a>
                    </li>
                    <li className="nav-item">
                      <a
                        aria-controls="dataPurging"
                        aria-selected="false"
                        className="nav-link px-2"
                        data-bs-target="#dataPurging"
                        data-bs-toggle="tab"
                        id="datapurge-tab"
                        role="tab"
                        type="button"
                      >
                        Data Purging
                      </a>
                    </li>
                    <li className="nav-item">
                      <a
                        aria-controls="emailSettings"
                        aria-selected="false"
                        className="nav-link px-2"
                        data-bs-target="#emailSettings"
                        data-bs-toggle="tab"
                        id="email-tab"
                        role="tab"
                        type="button"
                      >
                        Email Settings
                      </a>
                    </li>
                    <li className="nav-item">
                      <a
                        aria-controls="user_settings"
                        aria-selected="false"
                        className="nav-link px-2"
                        data-bs-target="#user_settings"
                        data-bs-toggle="tab"
                        id="user_settings-tab"
                        role="tab"
                        type="button"
                      >
                        User Settings
                      </a>
                    </li>
                    <div>




                    </div>
                    <li className="nav-item">
                      <a
                        aria-controls="permissions"
                        aria-selected="false"
                        className="nav-link px-2 disabled"
                        data-bs-target="#permissions"
                        data-bs-toggle="tab"
                        id="permissions-tab"
                        role="tab"
                        type="button"
                      >
                        Permissions
                      </a>
                    </li>
                    <li className="nav-item">
                      <a
                        aria-controls="group_settings"
                        aria-selected="false"
                        className="nav-link px-2"
                        data-bs-target="#group_settings"
                        data-bs-toggle="tab"
                        id="group_settings-tab"
                        role="tab"
                        type="button"
                      >
                        Group Settings
                      </a>
                    </li>
                    <li className="nav-item">
                      <a
                        aria-controls="blogs"
                        aria-selected="false"
                        className="nav-link px-2"
                        data-bs-target="#blogs"
                        data-bs-toggle="tab"
                        id="blogs-tab"
                        role="tab"
                        type="button"
                      >
                        Blogs 
                      </a>
                    </li>
                  </ul>
                </div>
              </div>
              <div
                className="tab-content card card-body px-3 py-0 w-100 overflow-auto"
                id="myTabContent"
                style={{ height: "calc(100vh - 20rem)" }}
              >
                <div
                  aria-labelledby="overview-tab"
                  className="tab-pane h-100 fade active show"
                  id="overview"
                  role="tabpanel"
                >
                  <div className="row h-100">
                    <div className="col border-end pt-6">
                      <Tree label={<Button1 variant="outlined">admin</Button1>}>
                        <TreeNode
                          label={
                            <Button1
                              variant="outlined"
                              style={{ color: "grey", borderColor: "#DEDEDE" }}
                              onClick={handleOpen}
                            >
                              Add New +
                            </Button1>
                          }
                        />
                        {groupList !== undefined && groupList?.data?.length > 0
                          ? groupList?.data?.map((group: any, _: number) => {
                              return (
                                <TreeNode
                                  key={_}
                                  label={
                                    <Button1
                                      variant="outlined"
                                      onClick={() => {
                                        setSelectedGroup(group);
                                      }}
                                    >
                                      {group?.name}
                                    </Button1>
                                  }
                                />
                              );
                            })
                          : ""}
                        {/* <TreeNode label={<Button1 variant="outlined">Group 1</Button1>}>
                          <TreeNode label={<Button1 variant="outlined">Group 1.1</Button1>}/>
                          <TreeNode label={<Button1 variant="outlined">Group 1.2</Button1>}>
                            <TreeNode label={<Button1 variant="outlined">Group 1.2.1</Button1>}/>
                            <TreeNode label={<Button1 variant="outlined">Group 1.2.2</Button1>}/>
                          </TreeNode>
                          <TreeNode label={<Button1 variant="outlined">Group 1.3</Button1>}/>
                        </TreeNode>
                        <TreeNode label={<Button1 variant="outlined">Group 2</Button1>}>
                          <TreeNode label={<Button1 variant="outlined">Group 2.1</Button1>}/>
                          <TreeNode label={<Button1 variant="outlined">Group 2.2</Button1>}/>
                        </TreeNode>
                        <TreeNode label={<Button1 variant="outlined">Group 3</Button1>}/>
                        <TreeNode label={<Button1 variant="outlined">Group 4</Button1>}>
                          <TreeNode label={<Button1 variant="outlined">Group 4.1</Button1>}>
                            <TreeNode label={<Button1 variant="outlined">Group 4.1.1</Button1>}>
                              <TreeNode label={<Button1 variant="outlined">Group 4.1.1.1</Button1>}/>
                              <TreeNode label={<Button1 variant="outlined">Group 4.1.1.2</Button1>}/>
                            </TreeNode>
                          </TreeNode>
                          <TreeNode label={<Button1 variant="outlined">Group 4.2</Button1>}/>
                        </TreeNode>
                        <TreeNode label={<Button1 variant="outlined">Group 5</Button1>}/> */}
                      </Tree>
                    </div>
                    <div className="col">
                      <div className="row h-50">
                        {/* <div className="container p-5">
                        </div> */}
                        <div className="container p-5">
                          <h5>Unresolved Alerts</h5>
                          <table className="table mt-5">
                            <tbody>
                              <tr>
                                <th scope="row">1</th>
                                <td>SCHEDULING_TASK_SYSTEMD_TIMERS</td>
                                <td>4238</td>
                              </tr>
                              <tr>
                                <th scope="row">2</th>
                                <td>IRB_SPAWNS_SHELL</td>
                                <td>434</td>
                              </tr>
                              <tr>
                                <th scope="row">3</th>
                                <td>SUDO_PRIVILEGED_COMMAND</td>
                                <td>123</td>
                              </tr>
                              <tr>
                                <th scope="row">4</th>
                                <td>SUDO_PRIVILEGED_COMMAND</td>
                                <td>898</td>
                              </tr>
                              <tr>
                                <th scope="row">5</th>
                                <td>SUDO_PRIVILEGED_COMMAND</td>
                                <td>612</td>
                              </tr>
                            </tbody>
                          </table>
                        </div>
                      </div>
                      <div className="row h-50">
                        <div className="container p-5">
                          {selectedGroup !== undefined ? (
                            <>
                              Group
                              <h5>
                                {selectedGroup !== undefined
                                  ? selectedGroup?.name
                                  : ""}
                              </h5>
                              <h5>
                                {selectedGroup !== undefined ? (
                                  <>
                                    {selectedGroup?.member_ids !== ""
                                      ? selectedGroup?.member_ids.split(",")
                                          .length
                                      : "0"}
                                    &nbsp;&nbsp;
                                    <i className="fa fa-group"></i>
                                  </>
                                ) : (
                                  <></>
                                )}
                              </h5>
                              <table className="table mt-5">
                                <thead>
                                  <tr>
                                    {/* <th scope="col"><Checkbox icon={<i className="fa fa-circle-o" aria-hidden="true"></i>} checkedIcon={<i className="fa fa-check-circle" aria-hidden="true"></i>}/></th> */}
                                    <th scope="col">Name</th>
                                    <th scope="col">Username</th>
                                    <th scope="col">Email</th>
                                    <th scope="col">Action</th>
                                  </tr>
                                </thead>
                                <tbody>
                                  {selectedGroup !== undefined ? (
                                    selectedGroup.member_ids
                                      .split(",")
                                      .map((id: any) => {
                                        return userList !== undefined ? (
                                          userList?.Data?.data?.map(
                                            (user: any, _: number) => {
                                              if (user.id === Number(id)) {
                                                return (
                                                  <tr key={_}>
                                                    {/* <td><Checkbox icon={<i className="fa fa-circle-o" aria-hidden="true"></i>} checkedIcon={<i className="fa fa-check-circle" aria-hidden="true"></i>}/></td> */}
                                                    <td>{user.name}</td>
                                                    <td>{user.username}</td>
                                                    <td>{user.email}</td>
                                                    <td>
                                                      <Chip
                                                        onClick={() => {
                                                          handleMembers(
                                                            "DELETE",
                                                            user,
                                                            selectedGroup
                                                          );
                                                        }}
                                                        label={"delete"}
                                                        color="primary"
                                                      />
                                                    </td>
                                                  </tr>
                                                );
                                              }
                                            }
                                          )
                                        ) : (
                                          <></>
                                        );
                                      })
                                  ) : (
                                    <></>
                                  )}

                                  {

                                  }
                                  {/* <tr>
                                <td><Checkbox icon={<i className="fa fa-circle-o" aria-hidden="true"></i>} checkedIcon={<i className="fa fa-check-circle" aria-hidden="true"></i>}/></td>
                                <td>Finish alert update</td>
                                <td><Chip label="New" color="primary" /></td>
                              </tr> */}
                                  {/* <tr>
                                <td><Checkbox icon={<i className="fa fa-circle-o" aria-hidden="true"></i>} checkedIcon={<i className="fa fa-check-circle" aria-hidden="true"></i>}/></td>
                                <td>Create new alert rule</td>
                                <td><Chip label="Urgent" color="error" /></td>
                              </tr>
                              <tr>
                                <td><Checkbox icon={<i className="fa fa-circle-o" aria-hidden="true"></i>} checkedIcon={<i className="fa fa-check-circle" aria-hidden="true"></i>}/></td>
                                <td>Update alert report</td>
                                <td><Chip label="Default" color="default" /></td>
                              </tr>
                              <tr>
                                <td><Checkbox icon={<i className="fa fa-circle-o" aria-hidden="true"></i>} checkedIcon={<i className="fa fa-check-circle" aria-hidden="true"></i>}/></td>
                                <td>Configure agent in 2 new systems</td>
                                <td><Chip label="New" color="primary" /></td>
                              </tr> */}
                                </tbody>
                              </table>
                            </>
                          ) : (
                            <></>
                          )}
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
                <div
                  aria-labelledby="dataPurging"
                  className="tab-pane h-100 fade"
                  id="dataPurging"
                  role="tabpanel"
                >
                  <DataPurge />
                  {/* Data Purging */}
                </div>
                <div
                  aria-labelledby="emailSettings"
                  className="tab-pane h-100 fade"
                  id="emailSettings"
                  role="tabpanel"
                >
                  <Email />
                  {/* Email Settings */}
                </div>
                <div
                  aria-labelledby="user_settings-tab"
                  className="tab-pane h-100 fade"
                  id="user_settings"
                  role="tabpanel"
                >
                  <Snackbar open={openSnakBar} autoHideDuration={6000}>
                    <Alert
                      onClose={handleClickAway}
                      severity="error"
                      sx={{ width: "100%" }}
                    >
                      Something went wrong<strong>!</strong>{" "}
                      <strong>UserName or Email might already exist</strong>
                    </Alert>
                  </Snackbar>
                  <div className="card-header bg-white border-0">
                    <div className="hstack">
                      <div className="ms-auto">
                        <button
                          type="button"
                          className="btn btn-primary-light align-middle text-primary"
                          onClick={() => {
                            setOpenAddUser(true);
                          }}
                        >
                          <i className="fa fa-plus me-2" aria-hidden="true"></i>
                          Add User
                        </button>
                      </div>
                    </div>
                    {/* <SecondaryTable
                      name="Users"
                      tableState={{
                        Data: userList?.Data?.data,
                        Error: userList?.Error,
                        Loading: userList?.Loading,
                      }}
                      columns={["id", "name", "username", "email"]}
                      Height={35}
                    /> */}
                    <table className="table table-hover">
                              <thead
                                className="bg-white"
                                style={{
                                  position: "sticky",
                                  top: 0,
                                  zIndex: 950,
                                }}
                              >
                                <tr className="fw-light text-muted">
                                  <th className="fw-light" scope="col">
                                    ID
                                  </th>
                                  <th className="fw-light" scope="col">
                                    Name:
                                  </th>
                                  <th className="fw-light" scope="col">
                                    User Name:
                                  </th>
                                  <th className="fw-light" scope="col">
                                    Email
                                  </th>
                                  <th className="fw-light" scope="col">
                                   
                                  </th>
                                  <th></th>
                                </tr>
                              </thead>
                              
                              <tbody className="border-top-0">
                                {userList?.Data?.data?.map(
                                  (user: any, index: any) => {
                                    return (
                                      <>
                                        <tr key={index}>
                                          <td>{user?.id}</td>
                                          <td>{user?.name}</td>
                                          <td>{user?.username}</td>
                                          <td>{user?.email}</td>
                                          <td>
                                            <div
                                              className="dropstart"
                                              onClick={() =>
                                                toggleOpenUser(index)
                                              }
                                            >
                                              <a
                                                type="button"
                                                id="dropdownMenuButton"
                                                data-bs-toggle="dropdown"
                                                aria-haspopup="true"
                                                aria-expanded="false"
                                              >
                                                <i className="fa fa-ellipsis-v"></i>
                                              </a>
                                              <div
                                                className={`dropdown-menu${
                                                  isMenuOpenUser &&
                                                  activeUser === index
                                                    ? " show"
                                                    : ""
                                                }`}
                                                aria-labelledby="dropdownMenuButton"
                                              >
                                                <a
                                                  className="dropdown-item"
                                                  type="button"
                                                  data-bs-toggle="modal"
                                                  data-bs-target="#ViewUserModal"
                                                  onClick={() => {}}
                                                >
                                                  View
                                                </a>
                                                <button
                                                  className="dropdown-item"
                                                  type="button"
                                                  data-bs-toggle="modal"
                                                  data-bs-target="#EditUserModal"
                                                  onClick={() => {
                                                    setEditUser(true);
                                                  }}
                                                >
                                                  Edit
                                                </button>
                                                <a
                                                  className="dropdown-item"
                                                  type="button"
                                                  onClick={() => {
                                                    deleteMe(
                                                      { id: user?.id },
                                                      DELETE_USER_URL,
                                                      setAlertData
                                                    );
                                                    wait(2);
                                                    getData(
                                                      GET_USER_URL,
                                                      setUserList
                                                    );
                                                  }}
                                                >
                                                  Delete
                                                </a>
                                              </div>
                                            </div>
                                          </td>
                                        </tr>
                                      </>
                                    );
                                  }
                                )}
                              </tbody>
                            </table>
                    {/* <div className="hstack">
                      <div className="ms-auto">
                        <button
                          type="button"
                          className="btn btn-primary align-middle text-white column1"
                          onClick={() => {
                            setOpenDeleteUser(true);
                          }}
                        >
                          <i
                            className="fa fa-minus me-2 column1"
                            aria-hidden="true"
                          ></i>
                          Delete User
                        </button>
                        
                        <button
                          type="button"
                          className="btn btn-primary align-middle text-white column1"
                          onClick={() => {
                            setOpenAddUser(true);
                          }}
                        >
                          <i
                            className="fa fa-minus me-2 "
                            aria-hidden="true"
                          ></i>
                          Edit User
                        </button>
                      </div>
                    </div> */}
                  </div>
                </div>
                <div
                  aria-labelledby="user_settings-tab"
                  className="tab-pane h-100 fade"
                  id="permissions"
                  role="tabpanel"
                >
                  {/* permissions */}
                </div>

                {/* Group settings */}
                <div
                  aria-labelledby="group_settings-tab"
                  className="tab-pane h-100 fade"
                  id="group_settings"
                  role="tabpanel"
                >
                  <div className="card py-3">
                    <div className="card-header bg-white border-0">
                      <div className="hstack">
                        <div>
                          <h4 className="fw-mid my-0">Groups</h4>
                        </div>
                        <div className="ms-auto">
                          <button
                            type="button"
                            className="btn btn-primary-light align-middle text-primary"
                            onClick={handleOpen}
                          >
                            <i
                              className="fa fa-plus me-2"
                              aria-hidden="true"
                            ></i>
                            Add Group
                          </button>
                        </div>
                      </div>
                    </div>

                    {groupList === undefined ||
                    groupList?.data?.length === 0 ? (
                      <>
                        <div className="card-body px-6">
                          <p className="text-center text-secondary fw-bold">
                            No records found.
                          </p>
                        </div>
                      </>
                    ) : (
                      <>
                        <div className="card-body px-6">
                          <div
                            className="w-100 overflow-auto"
                            id="#scrollbar-1"
                          >
                            <table className="table table-hover">
                              <thead
                                className="bg-white"
                                style={{
                                  position: "sticky",
                                  top: 0,
                                  zIndex: 950,
                                }}
                              >
                                <tr className="fw-light text-muted">
                                  <th className="fw-light" scope="col">
                                    Group Name:
                                  </th>
                                  <th className="fw-light" scope="col">
                                    No. of Members:
                                  </th>
                                  <th></th>
                                </tr>
                              </thead>
                              <tbody className="border-top-0">
                                {groupList?.data?.map(
                                  (group: any, index: any) => {
                                    return (
                                      <>
                                        <tr>
                                          <td>{group?.name}</td>
                                          <td>
                                            {group?.member_ids !== ""
                                              ? group?.member_ids.split(",")
                                                  .length
                                              : "0"}
                                            &nbsp;&nbsp;
                                            <i className="fa fa-group"></i>
                                          </td>
                                          <td>
                                            <div
                                              className="dropstart"
                                              onClick={() => toggleOpen(index)}
                                            >
                                              <a
                                                type="button"
                                                id="dropdownMenuButton"
                                                data-bs-toggle="dropdown"
                                                aria-haspopup="true"
                                                aria-expanded="false"
                                              >
                                                <i className="fa fa-ellipsis-v"></i>
                                              </a>
                                              <div
                                                className={`dropdown-menu${
                                                  isMenuOpen &&
                                                  activeGroup == index
                                                    ? " show"
                                                    : ""
                                                }`}
                                                aria-labelledby="dropdownMenuButton"
                                              >
                                                <a
                                                  className="dropdown-item"
                                                  type="button"
                                                  data-bs-toggle="modal"
                                                  data-bs-target="#ViewGroupsModal"
                                                  onClick={() => {}}
                                                >
                                                  View
                                                </a>
                                                <button
                                                  className="dropdown-item"
                                                  type="button"
                                                  onClick={() => {
                                                    deleteMe(
                                                      { id: group?.id },
                                                      DELETE_GROUP,
                                                      setAlertData
                                                    );
                                                    wait(5);
                                                    getData(
                                                      GET_GROUPS_URL,
                                                      setGroupList
                                                    );
                                                  }}
                                                >
                                                  Delete
                                                </button>
                                              </div>
                                            </div>
                                          </td>
                                        </tr>
                                      </>
                                    );
                                  }
                                )}
                              </tbody>
                            </table>
                          </div>
                        </div>
                      </>
                    )}
                  </div>
                </div>

                {/* blogs */}
                <div
                  aria-labelledby="blogs"
                  className="tab-pane h-100 fade"
                  id="blogs"
                  role="tabpanel"
                >
                  {/* <BlogList />
                   */}

                  <div className="card py-3">
                    <div className="card-header bg-white border-0">
                      <div className="hstack">
                        <div>
                          <h4 className="fw-mid my-0">Blogs</h4>
                        </div>
                        <div className="ms-auto">
                          <button
                            type="button"
                            className="btn btn-primary-light align-middle text-primary"
                            onClick={() => setOpenAddBlog(true)}
                          >
                            <i
                              className="fa fa-plus me-2"
                              aria-hidden="true"
                            ></i>
                            Add Blog
                          </button>
                        </div>
                      </div>
                    </div>

                    {blogsList === undefined ||
                    blogsList?.data?.length === 0 ? (
                      <>
                        <div className="card-body px-6">
                          <p className="text-center text-secondary fw-bold">
                            No records found.
                          </p>
                        </div>
                      </>
                    ) : (
                      <>
                        <div className="card-body px-6">
                          <div
                            className="w-100 overflow-auto"
                            id="#scrollbar-1"
                          >
                            <table className="table table-hover">
                              <thead
                                className="bg-white"
                                style={{
                                  position: "sticky",
                                  top: 0,
                                  zIndex: 950,
                                }}
                              >
                                <tr className="fw-light text-muted">
                                  <th className="fw-light" scope="col">
                                    Blog Name:
                                  </th>
                                  <th className="fw-light" scope="col">
                                    Creator:
                                  </th>
                                  <th className="fw-light" scope="col">
                                    Created At:
                                  </th>
                                  <th></th>
                                </tr>
                              </thead>
                              
                              <tbody className="border-top-0">
                                {blogsList?.data?.map(
                                  (blog: any, index: any) => {
                                    return (
                                      <>
                                        <tr key={index}>
                                          <td>{blog?.name}</td>
                                          <td>{blog?.creator}</td>
                                          <td>{blog?.created_at}</td>
                                          <td>
                                            <div
                                              className="dropstart"
                                              onClick={() =>
                                                toggleOpenBlog(index)
                                              }
                                            >
                                              <a
                                                type="button"
                                                id="dropdownMenuButton"
                                                data-bs-toggle="dropdown"
                                                aria-haspopup="true"
                                                aria-expanded="false"
                                              >
                                                <i className="fa fa-ellipsis-v"></i>
                                              </a>
                                              <div
                                                className={`dropdown-menu${
                                                  isMenuOpenBlog &&
                                                  activeBlog === index
                                                    ? " show"
                                                    : ""
                                                }`}
                                                aria-labelledby="dropdownMenuButton"
                                              >
                                                <a
                                                  className="dropdown-item"
                                                  type="button"
                                                  data-bs-toggle="modal"
                                                  data-bs-target="#ViewBlogsModal"
                                                  onClick={() => {}}
                                                >
                                                  View
                                                </a>
                                                <button
                                                  className="dropdown-item"
                                                  type="button"
                                                  data-bs-toggle="modal"
                                                  data-bs-target="#EditBlogsModal"
                                                  onClick={() => {
                                                    setEditBlog(true);
                                                  }}
                                                >
                                                  Edit
                                                </button>
                                                <a
                                                  className="dropdown-item"
                                                  type="button"
                                                  onClick={() => {
                                                    deleteMe(
                                                      { id: blog?.id },
                                                      BLOG_DELETE_URL,
                                                      setAlertData
                                                    );
                                                    wait(2);
                                                    getData(
                                                      GET_BLOGS_URL,
                                                      setBlogsList
                                                    );
                                                  }}
                                                >
                                                  Delete
                                                </a>
                                              </div>
                                            </div>
                                          </td>
                                        </tr>
                                      </>
                                    );
                                  }
                                )}
                              </tbody>
                            </table>
                          </div>
                        </div>
                      </>
                    )}
                  </div>
                </div>
              </div>
              {/* <!-- Modal --> */}
              <div
                aria-hidden="true"
                aria-labelledby="exampleModalLabel"
                className="modal fade"
                id="exampleModal"
                tabIndex={-1}
              >
                <div className="modal-dialog modal-dialog-centered">
                  <div className="modal-content">
                    <div className="modal-header">
                      <h5 className="modal-title" id="exampleModalLabel">
                        Alert Details
                      </h5>
                      <button
                        aria-label="Close"
                        className="btn-close"
                        data-bs-dismiss="modal"
                        type="button"
                      ></button>
                    </div>
                    <div className="modal-body">
                      <div></div>
                    </div>
                    <div className="modal-footer">
                      <button
                        className="btn btn-light"
                        data-bs-dismiss="modal"
                        type="button"
                      >
                        Close
                      </button>
                      <button className="btn btn-primary" type="button">
                        Take Action
                      </button>
                    </div>
                  </div>
                </div>
              </div>

              {/* Groups View Modal */}
              <div
                className="modal fade"
                id="ViewGroupsModal"
                tabIndex={-1}
                aria-labelledby="ViewGroupsModalLabel"
                aria-hidden="true"
              >
                <div className="modal-dialog view-rule-modal">
                  <div className="modal-content">
                    <div className="modal-header">
                      <h5 className="modal-title" id="ViewGroupsModal">
                        Group Settings
                      </h5>
                      <button
                        type="button"
                        className="btn-close"
                        data-bs-dismiss="modal"
                        aria-label="Close"
                      ></button>
                    </div>
                    <div className="modal-body">
                      <div className="container">
                        <div className="row align-items-start">
                          <div className="col">
                            <p className="fw-bold mb-0">Group Name</p>
                            <p>{groupList?.data?.[activeGroup]?.name}</p>
                          </div>
                          <div className="col">
                            <p className="fw-bold mb-0">Name</p>
                            <p>
                              {groupList?.data?.[activeGroup]?.member_ids !== ""
                                ? groupList?.data?.[
                                    activeGroup
                                  ]?.member_ids?.split(",").length
                                : "0"}
                              &nbsp; &nbsp;
                              <i className="fa fa-group"></i>
                            </p>
                          </div>
                          <div className="row h-50">
                            <div className="container p-5">
                              {activeGroup !== undefined ? (
                                <>
                                  <table className="table mt-5">
                                    <thead>
                                      <tr>
                                        {/* <th scope="col"><Checkbox icon={<i className="fa fa-circle-o" aria-hidden="true"></i>} checkedIcon={<i className="fa fa-check-circle" aria-hidden="true"></i>}/></th> */}
                                        <th scope="col">Name</th>
                                        <th scope="col">Username</th>
                                        <th scope="col">Email</th>
                                        <th scope="col">Action</th>
                                      </tr>
                                    </thead>
                                    
                                    {<tbody>
                                      {activeGroup !== DEFAULT_INDEX &&
                                      groupList !== undefined ? (
                                        groupList?.data?.[
                                          activeGroup
                                        ]?.member_ids
                                          ?.split(",")
                                          .map((id: any) => {
                                            return userList !== undefined ? (
                                              userList?.Data?.data?.map(
                                                (user: any, _: number) => {
                                                  if (user.id === Number(id)) {
                                                    return (
                                                      <tr key={_}>
                                                        {/* <td><Checkbox icon={<i className="fa fa-circle-o" aria-hidden="true"></i>} checkedIcon={<i className="fa fa-check-circle" aria-hidden="true"></i>}/></td> */}
                                                        <td>{user.name}</td>
                                                        <td>{user.username}</td>
                                                        <td>{user.email}</td>
                                                        <td>Hey</td>
                                                        {/* <td>
                                                          <Chip
                                                            onClick={() => {
                                                              handleMembers(
                                                                "DELETE",
                                                                user,
                                                                groupList
                                                                  ?.data?.[
                                                                  activeGroup
                                                                ]
                                                              );
                                                            }}
                                                            label={"delete"}
                                                            color="primary"
                                                          />
                                                        </td> */}
                                                      </tr>
                                                    );
                                                  }
                                                }
                                              )
                                            ) : (
                                              <></>
                                            );
                                          })
                                      ) : (
                                        <></>
                                      )}
                                    </tbody> }
                                  </table>
                                </>
                              ) : (
                                <></>
                              )}
                            </div>
                          </div>
                        </div>
                      </div>
                    </div>
                    <div className="modal-footer">
                      <button
                        type="button"
                        className="btn btn-secondary"
                        data-bs-dismiss="modal"
                      >
                        Close
                      </button>
                    </div>
                  </div>
                </div>
              </div>

              {/* Blog View Modal */}
              <div
                className="modal fade"
                id="ViewBlogsModal"
                tabIndex={-1}
                aria-labelledby="ViewBlogsModalLabel"
                aria-hidden="true"
              >
                <div className="modal-dialog view-rule-modal">
                  <div className="modal-content">
                    <div className="modal-header">
                      <h5
                        className="modal-title text-primary"
                        id="ViewBlogsModal"
                      >
                        Blog Details
                      </h5>
                      <button
                        type="button"
                        className="btn-close"
                        data-bs-dismiss="modal"
                        aria-label="Close"
                      ></button>
                    </div>
                    <div className="modal-body">
                      <div className="container">
                        <div className="row align-items-start">
                          <div className="row">
                            <p className="fw-bold mb-0">Title: </p>
                            <p>{blogsList?.data?.[activeBlog]?.name}</p>
                          </div>
                          <div className="row">
                            <p className="fw-bold mb-0">Content:</p>
                            <p>{blogsList?.data?.[activeBlog]?.content}</p>
                          </div>
                          <div className="row">
                            {blogsList?.data?.[activeBlog]?.image ? (
                              <>
                                <div className="col">
                                  <p className="fw-bold mb-0">Image:</p>
                                  <div className="col-12 col-md-8">
                                    <img
                                      className="w-100"
                                      src={blogsList?.data?.[activeBlog]?.image}
                                      alt={blogsList?.data?.[activeBlog]?.name}
                                    />
                                  </div>
                                </div>
                              </>
                            ) : (
                              <></>
                            )}
                          </div>
                          <div className="row">
                            <div className="col">
                              <p className="fw-bold mb-0">Created By:</p>
                              <p>{blogsList?.data?.[activeBlog]?.creator}</p>
                            </div>
                            <div className="col">
                              <p className="fw-bold mb-0">Created On:</p>
                              <p>{blogsList?.data?.[activeBlog]?.created_at}</p>
                            </div>
                          </div>
                        </div>
                      </div>
                    </div>
                    <div className="modal-footer">
                      <button
                        type="button"
                        className="btn btn-secondary"
                        data-bs-dismiss="modal"
                      >
                        Close
                      </button>
                    </div>
                  </div>
                </div>
              </div>
              {/* {user view modal} */}
              {

<div
                className="modal fade"
                id="ViewUserModal"
                tabIndex={-1}
                aria-labelledby="ViewUserModalLabel"
                aria-hidden="true"
              >
                <div className="modal-dialog view-rule-modal">
                  <div className="modal-content">
                    <div className="modal-header">
                      <h5
                        className="modal-title text-primary"
                        id="ViewUserModal"
                      >
                        User Details
                      </h5>
                      <button
                        type="button"
                        className="btn-close"
                        data-bs-dismiss="modal"
                        aria-label="Close"
                      ></button>
                    </div>
                    <div className="modal-body">
                      <div className="container">
                        <div className="row align-items-start">
                        <div className="row">
                            <p className="fw-bold mb-0">Name: </p>
                            <p>{userList?.Data?.data?.[activeUser]?.name}</p>
                          </div>
                          <div className="row">
                            <p className="fw-bold mb-0">UserName: </p>
                            <p>{userList?.Data?.data?.[activeUser]?.username}</p>
                          </div>
                          <div className="row">
                            <p className="fw-bold mb-0">Role: </p>
                            <p>{userList?.Data?.data?.[activeUser]?.role}</p>
                          </div>
                          <div className="row">
                            <p className="fw-bold mb-0">Email: </p>
                            <p>{userList?.Data?.data?.[activeUser]?.email}</p>
                          </div>
                            {/* {console.log("userlist.Data",userList?.data?.Data,activeUser)} */}
                          {/* <div className="row">
                            <p className="fw-bold mb-0">Content:</p>
                            <p>{userList?.Data?.[activeUser]?.content}</p>
                          </div> */}
                         
                          <div className="row">
                            {/* <div className="col">
                              <p className="fw-bold mb-0">Created By:</p>
                              <p>{userList?.Data?.[activeUser]?.creator}</p>
                            </div> */}
                            {/* <div className="col">
                              <p className="fw-bold mb-0">Created On:</p>
                              <p>{userList?.Data?.[activeUser]?.created_at}</p>
                            </div> */}
                          </div>
                        </div>
                      </div>
                    </div>
                    <div className="modal-footer">
                      <button
                        type="button"
                        className="btn btn-secondary"
                        data-bs-dismiss="modal"
                      >
                        Close
                      </button>
                    </div>
                  </div>
                </div>
              </div>

              }
              {/* <!-- Modal::end --> */}
            </div>
          </div>
        </div>
      </>
    );
  } else {
    if (user !== undefined) {
      return (
        <>
          <div className="card card-body">
            <div className="hstack">
              <div
                className="d-flex flex-column justify-content-start"
                style={{ width: "10vw", flexBasis: "20%" }}
              >
                <h3 className="fw-mid my-0">Personal Info</h3>
                <div>
                  <div className="mt-2">
                    <button
                      className="fs-8 px-1 btn btn-link text-muted"
                      // onClick={() => setispast(7)}
                    >
                      7 Days
                    </button>
                    <button
                      className="fs-8 px-1 btn btn-link text-muted active"
                      // onClick={() => setispast(15)}
                    >
                      15 Days
                    </button>
                    <button
                      className="fs-8 px-1 btn btn-link text-muted"
                      // onClick={() => setispast(180)}
                    >
                      180 Days
                    </button>
                  </div>
                </div>
              </div>
              <div className="row ms-5 w-100">
                <div className="col-2 border-end">
                  <div
                    className="row d-flex justify-content-center align-items-center"
                    style={{ fontSize: "1.3rem" }}
                  >
                    Unresolved
                  </div>
                  <div
                    className="row d-flex justify-content-center align-items-center"
                    style={{ fontSize: "1.8rem" }}
                  >
                    60
                  </div>
                </div>
                <div className="col-2 border-end">
                  <div
                    className="row d-flex justify-content-center align-items-center"
                    style={{ fontSize: "1.3rem" }}
                  >
                    Overdue
                  </div>
                  <div
                    className="row d-flex justify-content-center align-items-center"
                    style={{ fontSize: "1.8rem" }}
                  >
                    16
                  </div>
                </div>
                <div className="col-2 border-end">
                  <div
                    className="row d-flex justify-content-center align-items-center"
                    style={{ fontSize: "1.3rem" }}
                  >
                    Open
                  </div>
                  <div
                    className="row d-flex justify-content-center align-items-center"
                    style={{ fontSize: "1.8rem" }}
                  >
                    43
                  </div>
                </div>
                <div className="col-2">
                  <div
                    className="row d-flex justify-content-center align-items-center"
                    style={{ fontSize: "1.3rem" }}
                  >
                    On hold
                  </div>
                  <div
                    className="row d-flex justify-content-center align-items-center"
                    style={{ fontSize: "1.8rem" }}
                  >
                    64
                  </div>
                </div>
              </div>
            </div>
          </div>
          <div className="card py-3">
            <div className="card-body px-5">
              <div className="d-flex flex-column align-items-start">
                <div className="d-flex justify-content-between align-items-center gap-3 w-100">
                  <div className="d-flex justify-content-between align-items-center gap-0">
                    <ul className="nav nav-tabs justify-content-between mx-2">
                      <li className="nav-item">
                        <a
                          aria-controls="overview"
                          aria-selected="true"
                          className="nav-link px-2 active"
                          data-bs-target="#overview"
                          data-bs-toggle="tab"
                          id="overview-tab"
                          role="tab"
                          type="button"
                        >
                          Overview
                        </a>
                      </li>
                      <li className="nav-item">
                        <a
                          aria-controls="emailSettings"
                          aria-selected="false"
                          className="nav-link px-2"
                          data-bs-target="#emailSettings"
                          data-bs-toggle="tab"
                          id="email-tab"
                          role="tab"
                          type="button"
                        >
                          Email Settings
                        </a>
                      </li>
                      <li className="nav-item">
                        <a
                          aria-controls="permissions"
                          aria-selected="false"
                          className="nav-link px-2 disabled"
                          data-bs-target="#permissions"
                          data-bs-toggle="tab"
                          id="permissions-tab"
                          role="tab"
                          type="button"
                        >
                          Permissions
                        </a>
                      </li>
                      <li className="nav-item">
                        <a
                          aria-controls="groups"
                          aria-selected="false"
                          className="nav-link px-2 disabled"
                          data-bs-target="#groups"
                          data-bs-toggle="tab"
                          id="groups-tab"
                          role="tab"
                          type="button"
                        >
                          Groups
                        </a>
                      </li>
                      {/* <li className="nav-item">
                        <a
                          aria-controls="run-query"
                          aria-selected="false"
                          className="nav-link px-2"
                          data-bs-target="#run-query"
                          data-bs-toggle="tab"
                          id="tab-4"
                          role="tab"
                          type="button"
                        >
                          Run Query
                        </a>
                      </li> */}
                    </ul>
                  </div>
                </div>
                <div
                  className="tab-content card card-body px-3 py-0 w-100 overflow-auto"
                  id="myTabContent"
                  style={{ height: "calc(100vh - 24rem)" }}
                >
                  <div
                    aria-labelledby="tab-1"
                    className="tab-pane h-100 fade active show"
                    id="overview"
                    role="tabpanel"
                  >
                    <div className="row h-100">
                      <div className="col d-flex align-items-center border-end">
                        <div className="row">
                          <div className="col border-end">
                            <LineChart
                              width={600}
                              height={500}
                              data={data}
                              margin={{
                                top: 5,
                                right: 30,
                                left: 20,
                                bottom: 5,
                              }}
                            >
                              <Tooltip />
                              <Legend />
                              <XAxis dataKey="name" />
                              <YAxis />
                              <Line
                                type="monotone"
                                dataKey="completed"
                                stroke="#8884d8"
                                activeDot={{ r: 8 }}
                              />
                              <Line
                                type="monotone"
                                dataKey="assigned"
                                stroke="#82ca9d"
                              />
                            </LineChart>
                          </div>
                          <div className="col">
                            <div className="row h-100">
                              <div className="col-12 border-bottom">
                                <div className="row h-50">
                                  <div
                                    className="col d-flex justify-content-center align-items-end"
                                    style={{ color: "#9FA2B4" }}
                                  >
                                    <h5>Resolved</h5>
                                  </div>
                                </div>
                                <div className="row h-50">
                                  <div className="col d-flex justify-content-center align-items-start">
                                    <h2>449</h2>
                                  </div>
                                </div>
                              </div>
                              <div className="col-12 border-bottom">
                                <div className="row h-50">
                                  <div
                                    className="col d-flex justify-content-center align-items-end"
                                    style={{ color: "#9FA2B4" }}
                                  >
                                    <h5>Received</h5>
                                  </div>
                                </div>
                                <div className="row h-50">
                                  <div className="col d-flex justify-content-center align-items-start">
                                    <h2>426</h2>
                                  </div>
                                </div>
                              </div>
                              <div className="col-12 border-bottom">
                                <div className="row h-50">
                                  <div
                                    className="col d-flex justify-content-center align-items-end"
                                    style={{ color: "#9FA2B4" }}
                                  >
                                    <h5>Average first response time</h5>
                                  </div>
                                </div>
                                <div className="row h-50">
                                  <div className="col d-flex justify-content-center align-items-start">
                                    <h2>33m</h2>
                                  </div>
                                </div>
                              </div>
                              <div className="col-12">
                                <div className="row h-50">
                                  <div
                                    className="col d-flex justify-content-center align-items-end"
                                    style={{ color: "#9FA2B4" }}
                                  >
                                    <h5>Average response time</h5>
                                  </div>
                                </div>
                                <div className="row h-50">
                                  <div className="col d-flex justify-content-center align-items-start">
                                    <h2>449</h2>
                                  </div>
                                </div>
                              </div>
                            </div>
                          </div>
                        </div>
                      </div>
                      <div className="col">
                        <div className="row h-50">
                          {/* <div className="container p-5">
                          </div> */}
                          <div className="container p-5">
                            <h5>Unresolved Alerts</h5>
                            <table className="table mt-5">
                              <tbody>
                                <tr>
                                  <th scope="row">1</th>
                                  <td>SCHEDULING_TASK_SYSTEMD_TIMERS</td>
                                  <td>4238</td>
                                </tr>
                                <tr>
                                  <th scope="row">2</th>
                                  <td>IRB_SPAWNS_SHELL</td>
                                  <td>434</td>
                                </tr>
                                <tr>
                                  <th scope="row">3</th>
                                  <td>SUDO_PRIVILEGED_COMMAND</td>
                                  <td>123</td>
                                </tr>
                                <tr>
                                  <th scope="row">4</th>
                                  <td>SUDO_PRIVILEGED_COMMAND</td>
                                  <td>898</td>
                                </tr>
                                <tr>
                                  <th scope="row">5</th>
                                  <td>SUDO_PRIVILEGED_COMMAND</td>
                                  <td>612</td>
                                </tr>
                              </tbody>
                            </table>
                          </div>
                        </div>
                        <div className="row h-50">
                          <div className="container p-5">
                            <h5>Tasks</h5>
                            <table className="table mt-5">
                              <tbody>
                                <tr>
                                  <td>
                                    <Checkbox
                                      icon={
                                        <i
                                          className="fa fa-circle-o"
                                          aria-hidden="true"
                                        ></i>
                                      }
                                      checkedIcon={
                                        <i
                                          className="fa fa-check-circle"
                                          aria-hidden="true"
                                        ></i>
                                      }
                                    />
                                  </td>
                                  <td>Finish alert update</td>
                                  <td>
                                    <Chip label="New" color="primary" />
                                  </td>
                                </tr>
                                <tr>
                                  <td>
                                    <Checkbox
                                      icon={
                                        <i
                                          className="fa fa-circle-o"
                                          aria-hidden="true"
                                        ></i>
                                      }
                                      checkedIcon={
                                        <i
                                          className="fa fa-check-circle"
                                          aria-hidden="true"
                                        ></i>
                                      }
                                    />
                                  </td>
                                  <td>Create new alert rule</td>
                                  <td>
                                    <Chip label="Urgent" color="error" />
                                  </td>
                                </tr>
                                <tr>
                                  <td>
                                    <Checkbox
                                      icon={
                                        <i
                                          className="fa fa-circle-o"
                                          aria-hidden="true"
                                        ></i>
                                      }
                                      checkedIcon={
                                        <i
                                          className="fa fa-check-circle"
                                          aria-hidden="true"
                                        ></i>
                                      }
                                    />
                                  </td>
                                  <td>Update alert report</td>
                                  <td>
                                    <Chip label="Default" color="default" />
                                  </td>
                                </tr>
                                <tr>
                                  <td>
                                    <Checkbox
                                      icon={
                                        <i
                                          className="fa fa-circle-o"
                                          aria-hidden="true"
                                        ></i>
                                      }
                                      checkedIcon={
                                        <i
                                          className="fa fa-check-circle"
                                          aria-hidden="true"
                                        ></i>
                                      }
                                    />
                                  </td>
                                  <td>Configure agent in 2 new systems</td>
                                  <td>
                                    <Chip label="New" color="primary" />
                                  </td>
                                </tr>
                              </tbody>
                            </table>
                          </div>
                        </div>
                      </div>
                    </div>
                  </div>
                  <div
                    aria-labelledby="emailSettings"
                    className="tab-pane h-100 fade"
                    id="emailSettings"
                    role="tabpanel"
                  >
                    <Email />
                    {/* Email Settings */}
                  </div>
                  <div
                    aria-labelledby="tab-2"
                    className="tab-pane h-100 fade"
                    id="permissions-tab"
                    role="tabpanel"
                  >
                    {/* permissions */}
                  </div>
                  <div
                    aria-labelledby="tab-4"
                    className="tab-pane h-100 fade"
                    id="groups"
                    role="tabpanel"
                  >
                    {/* Groups */}
                  </div>
                </div>
                {/* <!-- Modal --> */}
                <div
                  aria-hidden="true"
                  aria-labelledby="exampleModalLabel"
                  className="modal fade"
                  id="exampleModal"
                  tabIndex={-1}
                >
                  <div className="modal-dialog modal-dialog-centered">
                    <div className="modal-content">
                      <div className="modal-header">
                        <h5 className="modal-title" id="exampleModalLabel">
                          Alert Details
                        </h5>
                        <button
                          aria-label="Close"
                          className="btn-close"
                          data-bs-dismiss="modal"
                          type="button"
                        ></button>
                      </div>
                      <div className="modal-body">
                        <div></div>
                      </div>
                      <div className="modal-footer">
                        <button
                          className="btn btn-light"
                          data-bs-dismiss="modal"
                          type="button"
                        >
                          Close
                        </button>
                        <button className="btn btn-primary" type="button">
                          Take Action
                        </button>
                      </div>
                    </div>
                  </div>
                </div>
                {/* <!-- Modal::end --> */}
              </div>
            </div>
          </div>
        </>
      );
    } else {
      return <></>;
    }
  }
};

export default Settings;
