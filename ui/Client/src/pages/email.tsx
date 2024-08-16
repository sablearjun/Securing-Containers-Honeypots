const Email = () => {
  return (
    <>
      <div className="card py-3 border-0">
        <div className="card-header bg-white border-0">
          <div className="hstack">
            <div>
              <h4 className="fw-mid my-0">Email</h4>
            </div>
          </div>
        </div>
        <div className="card-body px-6">
          <form id="email-form">
            <p className="mb-5">
              Configure sender's email and recipient's email address for
              receiving alerts.
            </p>
            <div className="form-group row mb-6 required">
              <label className="col-sm-2 col-form-label">Sender's email</label>
              <div className="col-sm-10">
                <input
                  type="text"
                  className="form-control is-invalid"
                  required
                />
                <div className="invalid-feedback">
                  Error On Sender's email Field
                </div>
              </div>
            </div>
            <div className="form-group row mb-6 required">
              <label className="col-sm-2 col-form-label">
                Sender's password
              </label>
              <div className="col-sm-10">
                <input type="password" className="form-control" required />
                <div className="invalid-feedback">
                  Error On Sender's password Field
                </div>
              </div>
            </div>
            <div className="form-group row mb-6 required">
              <label className="col-sm-2 col-form-label">SMTP Server</label>
              <div className="col-sm-10">
                <input
                  type="text"
                  placeholder="Eg: smtp.gmail.com"
                  className="form-control"
                  required
                />
                <div className="invalid-feedback">
                  Error On SMTP Server Field
                </div>
              </div>
            </div>
            <div className="form-group row mb-6 required">
              <label className="col-sm-2 col-form-label">SMTP Port</label>
              <div className="col-sm-10">
                <input
                  type="number"
                  placeholder="Eg:465"
                  className="form-control"
                  required
                />
                <div className="invalid-feedback">Error On SMTP Port Field</div>
              </div>
            </div>
            <div className="form-group row mb-6 required">
              <label className="col-sm-2 col-form-label">
                Email Recipients
              </label>
              <div className="col-sm-10">
                <textarea className="form-control" rows={4} required />
                <div className="invalid-feedback">
                  Error On Email Recipients Field
                </div>
              </div>
            </div>
            <div className="form-group row mb-6">
              <label className="col-sm-2 col-form-label">
                Connection Security
              </label>
              <div className="col-sm-10">
                <div className="form-check form-check-inline">
                  <input
                    className="form-check-input"
                    type="radio"
                    value="SSL"
                    id="ssl"
                    name="securityOptions"
                  />
                  <label className="form-check-label" htmlFor="ssl">
                    SSL
                  </label>
                </div>
                <div className="form-check form-check-inline">
                  <input
                    className="form-check-input"
                    type="radio"
                    value="TSL"
                    id="tsl"
                    name="securityOptions"
                  />
                  <label className="form-check-label" htmlFor="tsl">
                    TSL
                  </label>
                </div>
                <div className="form-check form-check-inline">
                  <input
                    className="form-check-input"
                    type="radio"
                    value="none"
                    id="none"
                    name="securityOptions"
                  />
                  <label className="form-check-label" htmlFor="none">
                    None
                  </label>
                </div>
              </div>
            </div>
            <div className="form-footer">
              <p>
                <span style={{ color: '#e32' }}>*</span> Mandatory Fields
              </p>
              <div className="mt-2 form-action w-25 mx-auto d-flex justify-content-evenly">
                <button type="submit" className="btn btn-primary">
                  Update
                </button>
                <button type="button" className="btn btn-secondary">
                  Test
                </button>
                <button type="reset" className="btn btn-light">
                  Cancel
                </button>
              </div>
            </div>
          </form>
        </div>
      </div>
    </>
  );
};

export default Email;
