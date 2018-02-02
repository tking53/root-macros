class GSAddback {
public:
  /** Default constructor setting things to zero */
  GSAddback() {
    energy = time = multiplicity = 0;
  }

  /** Default constructor setting default values
   * \param [in] ienergy : the initial energy
   * \param [in] itime : the initial time
   * \param [in] imultiplicity : multiplicity of the event */
  GSAddback(Double_t ienergy, Double_t itime, unsigned imultiplicity) {
    energy = ienergy;
    time = itime;
    multiplicity = imultiplicity;
  }

  Double_t energy;//!< Energy of the addback event
  Double_t time;//!< time of the addback event
  unsigned multiplicity;//!< multiplicity of the event
};
